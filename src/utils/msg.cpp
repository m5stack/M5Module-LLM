/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#include "../utils/msg.h"

using namespace m5_module_llm;

void ModuleMsg::init(ModuleComm* ModuleMsg)
{
    _module_comm = ModuleMsg;
}

void ModuleMsg::update(uint32_t timeout)
{
    auto reponse = _module_comm->getResponse(timeout);
    if (reponse.time_out) {
        return;
    }
    addMsgFromResponse(reponse.msg.c_str());
}

void ModuleMsg::addMsgFromResponse(const char* response)
{
    JsonDocument doc;
    if (deserializeJson(doc, response) != DeserializationError::Ok) {
        return;
    }

    ResponseMsg_t new_msg;
    new_msg.raw_msg       = response;
    new_msg.request_id    = doc["request_id"].as<String>();
    new_msg.work_id       = doc["work_id"].as<String>();
    new_msg.object        = doc["object"].as<String>();
    new_msg.error.code    = doc["error"]["code"];
    new_msg.error.message = doc["error"]["message"].as<String>();
    responseMsgList.push_back(new_msg);
}

void ModuleMsg::clearMsg(String request_id)
{
    for (auto iter = responseMsgList.begin(); iter != responseMsgList.end();) {
        if (iter->request_id == request_id) {
            iter = responseMsgList.erase(iter);
        } else {
            iter++;
        }
    }
}

bool ModuleMsg::takeMsg(String request_id, std::function<void(ResponseMsg_t& msg)> onMsg)
{
    bool ret = false;
    for (auto iter = responseMsgList.begin(); iter != responseMsgList.end();) {
        if (iter->request_id == request_id) {
            ret = true;
            if (onMsg) {
                onMsg(*iter);
            }
            iter = responseMsgList.erase(iter);
        } else {
            iter++;
        }
    }
    return ret;
}

bool ModuleMsg::waitAndTakeMsg(String request_id, std::function<void(ResponseMsg_t& msg)> onMsg, uint32_t timeout)
{
    uint32_t startTime = millis();
    while (millis() - startTime < timeout) {
        update();

        if (takeMsg(request_id, onMsg)) {
            return true;
        }

        delay(10);
    }
    return false;
}

bool ModuleMsg::sendCmdAndWaitToTakeMsg(const char* cmd, String request_id,
                                        std::function<void(ResponseMsg_t& msg)> onMsg, uint32_t timeout)
{
    _module_comm->sendCmd(cmd);
    clearMsg(request_id);
    return waitAndTakeMsg(request_id, onMsg, timeout);
}
