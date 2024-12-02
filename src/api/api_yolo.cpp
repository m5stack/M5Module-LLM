/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#include "api_yolo.h"

using namespace m5_module_llm;

void ApiYolo::init(ModuleMsg* moduleMsg)
{
    _module_msg = moduleMsg;
}

String ApiYolo::setup(ApiYoloSetupConfig_t config, String request_id)
{
    String cmd;
    {
        JsonDocument doc;
        doc["request_id"]              = request_id;
        doc["work_id"]                 = "yolo";
        doc["action"]                  = "setup";
        doc["object"]                  = "yolo.setup";
        doc["data"]["model"]           = config.model;
        doc["data"]["response_format"] = config.response_format;
        JsonArray inputArray           = doc["data"]["input"].to<JsonArray>();
        for (const String& str : config.input) {
            inputArray.add(str);
        }
        doc["data"]["enoutput"] = config.enoutput;
        serializeJson(doc, cmd);
    }

    String work_id;
    _module_msg->sendCmdAndWaitToTakeMsg(
        cmd.c_str(), request_id,
        [&work_id](ResponseMsg_t& msg) {
            // Copy work id
            work_id = msg.work_id;
        },
        5000);
    return work_id;
}

int ApiYolo::inference(String& work_id, uint8_t* input, size_t& raw_len, String request_id)
{
    String cmd;
    {
        JsonDocument doc;
        doc["RAW"]        = raw_len;
        doc["request_id"] = request_id;
        doc["work_id"]    = work_id;
        doc["action"]     = "inference";
        doc["object"]     = "cv.jpeg.base64";
        serializeJson(doc, cmd);
    }

    _module_msg->sendCmd(cmd.c_str());
    _module_msg->sendRaw(input, raw_len);
    return MODULE_LLM_OK;
}

int ApiYolo::inferenceAndWaitResult(String& work_id, uint8_t* input, size_t& raw_len,
                                    std::function<void(String&)> onResult, uint32_t timeout, String request_id)
{
    inference(work_id, input, raw_len, request_id);

    uint32_t time_out_count = millis();
    bool is_time_out        = false;
    bool is_msg_finish      = false;
    while (1) {
        _module_msg->update();
        _module_msg->takeMsg(request_id, [&time_out_count, &is_msg_finish, &onResult](ResponseMsg_t& msg) {
            String response_msg;
            {
                JsonDocument doc;
                deserializeJson(doc, msg.raw_msg);
                response_msg = doc["data"]["delta"].as<String>();
                if (!doc["data"]["finish"].isNull()) {
                    is_msg_finish = doc["data"]["finish"];
                    if (is_msg_finish) {
                        response_msg += '\n';
                    }
                }
            }
            if (onResult) {
                onResult(response_msg);
            }
            time_out_count = millis();
        });

        if (is_msg_finish) {
            break;
        }

        if (millis() - time_out_count > timeout) {
            is_time_out = true;
            break;
        }
    }

    if (is_time_out) {
        return MODULE_LLM_WAIT_RESPONSE_TIMEOUT;
    }
    return MODULE_LLM_OK;
}
