/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#include "api_whisper.h"

using namespace m5_module_llm;

void ApiWhisper::init(ModuleMsg* moduleMsg)
{
    _module_msg = moduleMsg;
}

String ApiWhisper::setup(ApiWhisperSetupConfig_t config, String request_id, String language)
{
    String cmd;
    {
        JsonDocument doc;
        doc["request_id"]              = request_id;
        doc["work_id"]                 = "whisper";
        doc["action"]                  = "setup";
        doc["object"]                  = "whisper.setup";
        doc["data"]["model"]           = config.model;
        doc["data"]["response_format"] = config.response_format;
        doc["data"]["language"]        = config.language;
        doc["data"]["enoutput"]        = config.enoutput;
        JsonArray inputArray           = doc["data"]["input"].to<JsonArray>();
        for (const String& str : config.input) {
            inputArray.add(str);
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

String ApiWhisper::exit(String work_id, String request_id)
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
