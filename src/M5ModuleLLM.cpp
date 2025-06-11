/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#include "M5ModuleLLM.h"
#include <ArduinoJson.h>

bool M5ModuleLLM::begin(Stream* serialPort)
{
    comm.init(serialPort);
    msg.init(&comm);
    sys.init(&msg);
    llm.init(&msg);
    vlm.init(&msg);
    audio.init(&msg);
    tts.init(&msg);
    melotts.init(&msg);
    kws.init(&msg);
    asr.init(&msg);
    yolo.init(&msg);
    camera.init(&msg);
    vad.init(&msg);
    whisper.init(&msg);
    depthanything.init(&msg);
    return true;
}

bool M5ModuleLLM::checkConnection()
{
    const bool result = (sys.ping() == MODULE_LLM_OK);
    llm_version       = sys.version();
    return result;
}

bool M5ModuleLLM::setBaudRate(uint32_t baudRate)
{
    return sys.setBaudRate(baudRate) == MODULE_LLM_OK;
}

void M5ModuleLLM::update()
{
    msg.update();
    // _module_msg.responseMsgList.clear();
}
