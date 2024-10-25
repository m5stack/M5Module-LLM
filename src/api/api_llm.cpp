/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#include "api_llm.h"

using namespace m5_module_llm;

void ApiLlm::init(ModuleMsg* moduleMsg)
{
    _module_msg = moduleMsg;
}

String ApiLlm::setup(ApiLlmSetupConfig_t config, String request_id)
{
    String cmd;
    {
        JsonDocument doc;
        doc["request_id"]              = request_id;
        doc["work_id"]                 = "llm";
        doc["action"]                  = "setup";
        doc["object"]                  = "llm.setup";
        doc["data"]["model"]           = config.model;
        doc["data"]["response_format"] = config.response_format;
        doc["data"]["input"]           = config.input;
        doc["data"]["enoutput"]        = config.enoutput;
        doc["data"]["enkws"]           = config.enkws;
        doc["data"]["max_token_len"]   = config.max_token_len;
        doc["data"]["prompt"]          = config.prompt;
        serializeJson(doc, cmd);
    }

    String llm_work_id;
    _module_msg->sendCmdAndWaitToTakeMsg(
        cmd.c_str(), request_id,
        [&llm_work_id](ResponseMsg_t& msg) {
            // Copy work id
            llm_work_id = msg.work_id;
        },
        10000);
    return llm_work_id;
}

int ApiLlm::inference(String work_id, String input, String request_id)
{
    String cmd;
    {
        JsonDocument doc;
        doc["request_id"]     = request_id;
        doc["work_id"]        = work_id;
        doc["action"]         = "inference";
        doc["object"]         = "llm.utf-8.stream";
        doc["data"]["delta"]  = input;
        doc["data"]["index"]  = 0;
        doc["data"]["finish"] = true;
        serializeJson(doc, cmd);
    }

    _module_msg->sendCmd(cmd.c_str());
    return MODULE_LLM_OK;
}

int ApiLlm::inferenceAndWaitResult(String work_id, String input, std::function<void(String&)> onResult,
                                   uint32_t timeout, String request_id)
{
    inference(work_id, input, request_id);

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
