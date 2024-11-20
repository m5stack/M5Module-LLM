/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#include "api_asr.h"

using namespace m5_module_llm;

void ApiAsr::init(ModuleMsg* moduleMsg)
{
    _module_msg = moduleMsg;
}

String ApiAsr::setup(ApiAsrSetupConfig_t config, String request_id)
{
    String cmd;
    {
        JsonDocument doc;
        doc["request_id"]              = request_id;
        doc["work_id"]                 = "asr";
        doc["action"]                  = "setup";
        doc["object"]                  = "asr.setup";
        doc["data"]["model"]           = config.model;
        doc["data"]["response_format"] = config.response_format;
        JsonArray inputArray           = doc["data"]["input"].to<JsonArray>();
        for (const String& str : config.input)
        {
            inputArray.add(str);
        }
        doc["data"]["enoutput"]        = config.enoutput;
        doc["data"]["rule1"]           = config.rule1;
        doc["data"]["rule2"]           = config.rule2;
        doc["data"]["rule3"]           = config.rule3;
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
