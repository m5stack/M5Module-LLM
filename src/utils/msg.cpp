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

void ModuleMsg::update()
{
    // 拉取串口响应
    auto reponse = _module_comm->getResponse(50);
    if (reponse.time_out) {
        return;
    }
    addMsgFromResponse(reponse.msg.c_str());
}

void ModuleMsg::addMsgFromResponse(const char* response)
{
    // 尝试解析
    JsonDocument doc;
    if (deserializeJson(doc, response) != DeserializationError::Ok) {
        return;
    }

    // 压进消息列表
    ResponseMsg_t new_msg;
    new_msg.raw_msg       = response;
    new_msg.request_id    = doc["request_id"].as<String>();
    new_msg.work_id       = doc["work_id"].as<String>();
    new_msg.object        = doc["object"].as<String>();
    new_msg.error.code    = doc["error"]["code"];
    new_msg.error.message = doc["error"]["message"].as<String>();
    responseMsgList.push_back(new_msg);
    // printf("[\033[1;34mdebug\033[0m] get msg:\n%s\n", response);
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
    // 遍历消息列表
    for (auto iter = responseMsgList.begin(); iter != responseMsgList.end();) {
        // 匹对 id
        // printf("%s %s %d\n", workId.c_str(), iter->workId.c_str(), iter->workId == workId);
        if (iter->request_id == request_id) {
            ret = true;
            // 触发回调
            if (onMsg) {
                onMsg(*iter);
            }
            // 移出列表
            iter = responseMsgList.erase(iter);
        } else {
            iter++;
        }
    }
    return ret;
}

bool ModuleMsg::waitAndTakeMsg(String request_id, std::function<void(ResponseMsg_t& msg)> onMsg, uint32_t timeout)
{
    uint32_t time_out_count = millis();
    bool is_time_out        = false;
    while (1) {
        // 刷新消息拉取
        update();

        // 匹配
        if (takeMsg(request_id, onMsg)) {
            break;
        }

        // 超时判断
        if (millis() - time_out_count > timeout) {
            is_time_out = true;
            break;
        }
    }
    return !is_time_out;
}

bool ModuleMsg::sendCmdAndWaitToTakeMsg(const char* cmd, String request_id,
                                        std::function<void(ResponseMsg_t& msg)> onMsg, uint32_t timeout)
{
    _module_comm->sendCmd(cmd);
    clearMsg(request_id);
    return waitAndTakeMsg(request_id, onMsg, timeout);
}
