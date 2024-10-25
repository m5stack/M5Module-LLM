/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#include "api_audio.h"

using namespace m5_module_llm;

void ApiAudio::init(ModuleMsg* moduleMsg)
{
    _module_msg = moduleMsg;
}

String ApiAudio::setup(ApiAudioSetupConfig_t config, String request_id)
{
    String cmd;
    {
        JsonDocument doc;
        doc["request_id"]         = request_id;
        doc["work_id"]            = "audio";
        doc["action"]             = "setup";
        doc["object"]             = "audio.setup";
        doc["data"]["capcard"]    = config.capcard;
        doc["data"]["capdevice"]  = config.capdevice;
        doc["data"]["capVolume"]  = config.capVolume;
        doc["data"]["playcard"]   = config.playcard;
        doc["data"]["playdevice"] = config.playdevice;
        doc["data"]["playVolume"] = config.playVolume;
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
