/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once
#include <Arduino.h>
#include <vector>
#include <map>

namespace m5_module_llm {

/**
 * @brief Handles communication between the device and ModuleLLM.
 *
 * This class provides methods to send commands and receive responses
 * from the ModuleLLM through the serial stream
 */
class ModuleComm {
public:
    struct Respond_t {
        bool time_out = false;
        String msg;
    };

    bool init(Stream* serialPort);
    void sendCmd(const char* cmd);
    void sendRaw(const uint8_t* data, size_t& raw_len);
    Respond_t getResponse(uint32_t timeout = 0xFFFFFFFF);

private:
    Stream* _serial = nullptr;
};

/**
 * @brief Serial passthrough for debugging
 *
 * @param port1
 * @param port2
 * @param prettyJson
 */
void SerialPassthroughUpdate(Stream* port1, Stream* port2, bool prettyJson = false);

}  // namespace m5_module_llm
