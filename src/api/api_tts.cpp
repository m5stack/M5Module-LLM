/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#include "api_tts.h"

using namespace m5_module_llm;

void ApiTts::init(ModuleMsg* moduleMsg)
{
    _module_msg = moduleMsg;
}

String ApiTts::setup(ApiTtsSetupConfig_t config, String request_id)
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
        doc["data"]["input"]           = config.input;
        doc["data"]["enoutput"]        = config.enoutput;
        doc["data"]["enkws"]           = config.enkws;
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
        doc["data"]["index"]  = 1;
        doc["data"]["finish"] = true;
        serializeJson(doc, cmd);
    }

    if (timeout == 0) {
        _module_msg->sendCmd(cmd.c_str());
        return MODULE_LLM_OK;
    }

    int ret          = MODULE_LLM_WAIT_RESPONSE_TIMEOUT;
    _module_msg->sendCmdAndWaitToTakeMsg(
        cmd.c_str(), request_id,
        [&ret](ResponseMsg_t& msg) {
            // Copy error code
            ret = msg.error.code;
        },
        timeout);
    return ret;
}
