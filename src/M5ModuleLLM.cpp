/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#include "M5ModuleLLM.h"
#include <ArduinoJson.h>
#include <mooncake_log.h>

using namespace mooncake;

bool M5ModuleLLM::begin(Stream* serialPort)
{
    comm.init(serialPort);
    msg.init(&comm);
    sys.init(&msg);
    llm.init(&msg);
    audio.init(&msg);
    tts.init(&msg);
    kws.init(&msg);
    asr.init(&msg);
    return true;
}

bool M5ModuleLLM::checkConnection()
{
    return sys.ping() == MODULE_LLM_OK;
}

void M5ModuleLLM::update()
{
    msg.update();
    // _module_msg.responseMsgList.clear();
}
