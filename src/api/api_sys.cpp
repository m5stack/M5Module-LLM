/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#include "api_sys.h"

using namespace m5_module_llm;

static const char* _cmd_ping =
    "{\"request_id\":\"sys_ping\",\"work_id\":\"sys\",\"action\":\"ping\",\"object\":\"None\",\"data\":\"None\"}";
static const char* _cmd_reset =
    "{\"request_id\":\"sys_reset\",\"work_id\":\"sys\",\"action\":\"reset\",\"object\":\"None\",\"data\":\"None\"}";
static const char* _cmd_reboot =
    "{\"request_id\":\"sys_reboot\",\"work_id\":\"sys\",\"action\":\"reboot\",\"object\":\"None\",\"data\":\"None\"}";
static const char* _cmd_ls_mode =
    "{\"request_id\":\"sys_lsmode\",\"work_id\":\"sys\",\"action\":\"lsmode\",\"object\":\"None\",\"data\":\"None\"}";

void ApiSys::init(ModuleMsg* moduleMsg)
{
    _module_msg = moduleMsg;
}

int ApiSys::ping()
{
    int ret = MODULE_LLM_WAIT_RESPONSE_TIMEOUT;
    _module_msg->sendCmdAndWaitToTakeMsg(
        _cmd_ping, "sys_ping", [&ret](ResponseMsg_t& msg) { ret = msg.error.code; }, 2000);
    return ret;
}

int ApiSys::reset(bool waitResetFinish)
{
    int ret = MODULE_LLM_WAIT_RESPONSE_TIMEOUT;

    // Send cmd and wait ok response
    _module_msg->sendCmdAndWaitToTakeMsg(
        _cmd_reset, "sys_reset", [&ret](ResponseMsg_t& msg) { ret = msg.error.code; }, 2000);
    if (ret != MODULE_LLM_OK) {
        return ret;
    }

    // Wait reset finish response
    if (waitResetFinish) {
        ret = MODULE_LLM_WAIT_RESPONSE_TIMEOUT;
        _module_msg->responseMsgList.clear();
        _module_msg->waitAndTakeMsg(
            "0", [&ret](ResponseMsg_t& msg) { ret = msg.error.code; }, 15000);
    }
    return ret;
}

int ApiSys::reboot()
{
    int ret = MODULE_LLM_WAIT_RESPONSE_TIMEOUT;
    _module_msg->sendCmdAndWaitToTakeMsg(
        _cmd_reboot, "sys_reboot", [&ret](ResponseMsg_t& msg) { ret = msg.error.code; }, 2000);
    return ret;
}
