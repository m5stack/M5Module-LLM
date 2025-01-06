/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#include "api_camera.h"

using namespace m5_module_llm;

void ApiCamera::init(ModuleMsg* moduleMsg)
{
    _module_msg = moduleMsg;
}

String ApiCamera::setup(ApiCameraSetupConfig_t config, String request_id)
{
    String cmd;
    {
        JsonDocument doc;
        doc["request_id"]              = request_id;
        doc["work_id"]                 = "camera";
        doc["action"]                  = "setup";
        doc["object"]                  = "camera.setup";
        doc["data"]["response_format"] = config.response_format;
        doc["data"]["input"]           = config.input;
        doc["data"]["enoutput"]        = config.enoutput;
        doc["data"]["frame_width"]     = config.frame_width;
        doc["data"]["frame_height"]    = config.frame_height;
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

String ApiCamera::exit(String work_id, String request_id)
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
