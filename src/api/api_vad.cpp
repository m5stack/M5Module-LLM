/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#include "api_vad.h"

using namespace m5_module_llm;

void ApiVad::init(ModuleMsg* moduleMsg)
{
    _module_msg = moduleMsg;
}

String ApiVad::setup(ApiVadSetupConfig_t config, String request_id)
{
    String cmd;
    {
        JsonDocument doc;
        doc["request_id"]              = request_id;
        doc["work_id"]                 = "vad";
        doc["action"]                  = "setup";
        doc["object"]                  = "vad.setup";
        doc["data"]["model"]           = config.model;
        doc["data"]["response_format"] = config.response_format;
        doc["data"]["enoutput"]        = config.enoutput;
        JsonArray inputArray           = doc["data"]["input"].to<JsonArray>();
        for (const String& str : config.input) {
            inputArray.add(str);
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
        30000);
    return work_id;
}

String ApiVad::exit(String work_id, String request_id)
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
