/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once
#include "comm.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <functional>
#include <vector>

enum ModuleLLMErrorCode_t {
    MODULE_LLM_OK                              = 0,
    MODULE_LLM_RESET_WARN                      = -1,
    MODULE_LLM_JSON_FORMAT_ERROR               = -2,
    MODULE_LLM_ACTION_MATCH_FAILED             = -3,
    MODULE_LLM_INFERENCE_DATA_PUSH_FAILED      = -4,
    MODULE_LLM_MODEL_LOADING_FAILED            = -5,
    MODULE_LLM_UNIT_NOT_EXIST                  = -6,
    MODULE_LLM_UNKNOWN_OPERATION               = -7,
    MODULE_LLM_UNIT_RESOURCE_ALLOCATION_FAILED = -8,
    MODULE_LLM_UNIT_CALL_FAILED                = -9,
    MODULE_LLM_MODEL_INIT_FAILED               = -10,
    MODULE_LLM_MODEL_RUN_FAILED                = -11,
    MODULE_LLM_MODULE_NOT_INITIALISED          = -12,
    MODULE_LLM_MODULE_ALREADY_WORKING          = -13,
    MODULE_LLM_MODULE_NOT_WORKING              = -14,
    MODULE_LLM_NO_UPDATEABLE_MODULES           = -15,
    MODULE_LLM_NO_MODULES_AVAILABLE_FOR_UPDATE = -16,
    MODULE_LLM_FILE_OPEN_FAILED                = -17,
    MODULE_LLM_WAIT_RESPONSE_TIMEOUT           = -97,
    MODULE_LLM_RESPONSE_PARSE_FAILED           = -98,
    MODULE_LLM_ERROR_NONE                      = -99,
};

namespace m5_module_llm {

struct Error_t {
    int16_t code = MODULE_LLM_ERROR_NONE;
    String message;
};

struct ResponseMsg_t {
    String raw_msg;
    String request_id;
    String work_id;
    String object;
    Error_t error;
};

class ModuleMsg {
public:
    void init(ModuleComm* moduleComm);
    void update(uint32_t timeout =50);

    inline void sendCmd(const char* cmd)
    {
        _module_comm->sendCmd(cmd);
    }

    inline void sendRaw(const uint8_t* data, size_t& raw_len)
    {
        _module_comm->sendRaw(data, raw_len);
    }
    /**
     * @brief Module response message list
     *
     */
    std::vector<ResponseMsg_t> responseMsgList;

    void addMsgFromResponse(const char* response);
    void clearMsg(String request_id);
    bool takeMsg(String request_id, std::function<void(ResponseMsg_t& msg)> onMsg);
    bool waitAndTakeMsg(String request_id, std::function<void(ResponseMsg_t&)> onMsg, uint32_t timeout = 10000);
    bool sendCmdAndWaitToTakeMsg(const char* cmd, String request_id, std::function<void(ResponseMsg_t& msg)> onMsg,
                                 uint32_t timeout = 10000);

private:
    ModuleComm* _module_comm = nullptr;
};

}  // namespace m5_module_llm
