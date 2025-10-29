/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#include "comm.h"
#include <ArduinoJson.h>

using namespace m5_module_llm;
const size_t JSON_BUFFER_SIZE = 2048;
char jsonBuffer[JSON_BUFFER_SIZE];

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
    ret.time_out = false;
    static String buffer;
    uint32_t startTime = millis();
    static int openBraces     = 0;
    bool started       = false;

    while (millis() - startTime < timeout) {
        while (_serial->available()) {
            char c = (char)_serial->read();
            if(c != '\n'){
                buffer += c;
                if(c == '{'){
                    openBraces ++;
                }else if (c == '}') {
                    openBraces --;
                    if(openBraces == 0){
                        ret.msg = buffer;
                        buffer.clear();
                        return ret;
                    }else if (openBraces < 0) {
                        openBraces = 0;
                        buffer.clear();
                    }
                }
            }
            else{
                openBraces = 0;
                buffer.clear();
            }
        }
        delay(1);
    }
    ret.time_out = true;
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

