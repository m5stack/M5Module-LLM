/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#include "api_tts.h"
#include "api_version.h"

using namespace m5_module_llm;

void ApiTts::init(ModuleMsg* moduleMsg)
{
    _module_msg = moduleMsg;
}

String ApiTts::setup(ApiTtsSetupConfig_t config, String request_id, String language)
{
    String cmd;
    {
        JsonDocument doc;
        doc["request_id"]              = request_id;
        doc["work_id"]                 = "tts";
        doc["action"]                  = "setup";
        doc["object"]                  = "tts.setup";
        doc["data"]["model"]           = config.model;
        doc["data"]["response_format"] = config.response_format;
        doc["data"]["enoutput"]        = config.enoutput;
        doc["data"]["enkws"]           = config.enkws;
        doc["data"]["enaudio"]         = config.enaudio;
        if (llm_version == "v1.0") {
            doc["data"]["response_format"] = "tts.base64.wav";
            doc["data"]["input"]           = config.input[0];
            doc["data"]["enoutput"]        = true;
        } else {
            JsonArray inputArray = doc["data"]["input"].to<JsonArray>();
            for (const String& str : config.input) {
                inputArray.add(str);
            }
        }
        float version = llm_version.substring(1).toFloat();
        if (version >= 1.6) {
            if (language == "zh_CN") {
                doc["data"]["model"] = "single-speaker-fast";
            } else {
                doc["data"]["model"] = "single-speaker-english-fast";
            }
        } else {
            if (language == "zh_CN") {
                doc["data"]["model"] = "single_speaker_fast";
            }
        }

        for (const auto& pair : config.extra_params) {
            const String& key   = pair.first;
            const String& value = pair.second;

            if (value == "bool_true") {
                doc["data"][key] = true;
            } else if (value == "bool_false") {
                doc["data"][key] = false;
            } else if (value.indexOf('.') != -1) {
                doc["data"][key] = value.toFloat();
            } else if (value.length() > 0 && (isDigit(value.charAt(0)) || value.charAt(0) == '-')) {
                doc["data"][key] = value.toInt();
            } else {
                doc["data"][key] = value;
            }
        }
        serializeJson(doc, cmd);
    }

    String work_id;
    _module_msg->sendCmdAndWaitToTakeMsg(
        cmd.c_str(), request_id,
        [&work_id](ResponseMsg_t& msg) {
            // Copy work id
            work_id = msg.work_id;
        },
        10000);
    return work_id;
}

String ApiTts::exit(String work_id, String request_id)
{
    String cmd;
    {
        JsonDocument doc;
        doc["request_id"] = request_id;
        doc["work_id"]    = work_id;
        doc["action"]     = "exit";
        serializeJson(doc, cmd);
    }

    _module_msg->sendCmdAndWaitToTakeMsg(
        cmd.c_str(), request_id,
        [&work_id](ResponseMsg_t& msg) {
            // Copy work id
            work_id = msg.work_id;
        },
        100);
    return work_id;
}

int ApiTts::inference(String work_id, String input, uint32_t timeout, String request_id)
{
    String cmd;
    {
        JsonDocument doc;
        doc["request_id"]     = request_id;
        doc["work_id"]        = work_id;
        doc["action"]         = "inference";
        doc["object"]         = "tts.utf-8.stream";
        doc["data"]["delta"]  = input;
        doc["data"]["index"]  = 0;
        doc["data"]["finish"] = true;
        serializeJson(doc, cmd);
    }

    if (timeout == 0) {
        _module_msg->sendCmd(cmd.c_str());
        return MODULE_LLM_OK;
    }

    int ret = MODULE_LLM_WAIT_RESPONSE_TIMEOUT;
    _module_msg->sendCmdAndWaitToTakeMsg(
        cmd.c_str(), request_id,
        [&ret](ResponseMsg_t& msg) {
            // Copy error code
            ret = msg.error.code;
        },
        timeout);
    return ret;
}
