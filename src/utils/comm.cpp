/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#include "comm.h"
#include <ArduinoJson.h>

using namespace m5_module_llm;

bool ModuleComm::init(Stream* serialPort)
{
    if (!serialPort) {
        return false;
    }
    _serial = serialPort;
    while (_serial->available()) {
        _serial->read();
    }
    return true;
}

void ModuleComm::sendCmd(const char* cmd)
{
    // printf("[\033[1;34mdebug\033[0m] send cmd:\n%s\n", cmd);
    _serial->print(cmd);
}

void ModuleComm::sendRaw(const uint8_t* data, size_t& raw_len)
{
    _serial->write(data, raw_len);
}

ModuleComm::Respond_t ModuleComm::getResponse(uint32_t timeout)
{
    Respond_t ret;

    uint32_t time_out_count = millis();
    bool get_msg            = false;
    uint32_t get_msg_count  = millis();
    while (1) {
        // Check input
        if (_serial->available()) {
            get_msg = true;
            while (_serial->available()) {
                ret.msg += (char)_serial->read();
            }
            get_msg_count  = millis();
            time_out_count = millis();
        }

        // Check package finish, if more than 50ms no input, treat it as a package
        else if (get_msg) {
            if (millis() - get_msg_count > 50) {
                break;
            }
        }

        // Check timeout
        if (millis() - time_out_count > timeout) {
            ret.time_out = true;
            break;
        }

        delay(5);
    }

    return ret;
}

void m5_module_llm::SerialPassthroughUpdate(Stream* port1, Stream* port2, bool prettyJson)
{
    if (port1->available()) {
        port1->print("<<<\n");
        while (port1->available()) {
            port2->write(port1->read());
        }
    }

    if (prettyJson) {
        static JsonDocument doc;
        DeserializationError error = deserializeJson(doc, *port2);
        if (error != DeserializationError::Ok) {
            return;
        }
        port1->print(">>>\n\n");
        serializeJsonPretty(doc, *port1);
        port1->print("\n\n");
    } else {
        if (port2->available()) {
            port1->print(">>>\n\n");
            while (port2->available()) {
                port1->write(port2->read());
            }
            port1->print("\n\n");
        }
    }
}
