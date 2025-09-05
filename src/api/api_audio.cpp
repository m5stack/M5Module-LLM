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
        5000);
    return work_id;
}

String ApiAudio::exit(String work_id, String request_id)
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
