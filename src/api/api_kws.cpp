/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#include "api_kws.h"

using namespace m5_module_llm;

void ApiKws::init(ModuleMsg* moduleMsg)
{
    _module_msg = moduleMsg;
}

String ApiKws::setup(ApiKwsSetupConfig_t config, String request_id)
{
    String cmd;
    {
        JsonDocument doc;
        doc["request_id"]              = request_id;
        doc["work_id"]                 = "kws";
        doc["action"]                  = "setup";
        doc["object"]                  = "kws.setup";
        doc["data"]["model"]           = config.model;
        doc["data"]["response_format"] = config.response_format;
        doc["data"]["input"]           = config.input;
        doc["data"]["enoutput"]        = config.enoutput;
        doc["data"]["kws"]             = config.kws;
        serializeJson(doc, cmd);
    }

    String work_id;
    bool is_time_out = _module_msg->sendCmdAndWaitToTakeMsg(
        cmd.c_str(), request_id,
        [&work_id](ResponseMsg_t& msg) {
            // Copy work id
            work_id = msg.work_id;
        },
        30000);
    return work_id;
}
