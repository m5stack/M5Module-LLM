/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#include <Arduino.h>
#include <M5Unified.h>
#include <M5ModuleLLM.h>

#define CommSerialPort Serial

M5ModuleLLM module_llm;
String llm_work_id;
String received_question;
bool question_ok;

void setup()
{
    M5.begin();
    M5.Display.setTextSize(2);
    M5.Display.setTextScroll(true);
    // M5.Display.setFont(&fonts::efontCN_12);  // Support Chinese display

    /* Init usb serial */
    CommSerialPort.begin(115200);

    /* Init module serial port */
    // int rxd = 16, txd = 17;  // Basic
    // int rxd = 13, txd = 14;  // Core2
    // int rxd = 18, txd = 17;  // CoreS3
    int rxd = M5.getPin(m5::pin_name_t::port_c_rxd);
    int txd = M5.getPin(m5::pin_name_t::port_c_txd);
    Serial2.begin(115200, SERIAL_8N1, rxd, txd);

    /* Init module */
    module_llm.begin(&Serial2);

    /* Make sure module is connected */
    M5.Display.printf(">> Check ModuleLLM connection..\n");
    while (1) {
        if (module_llm.checkConnection()) {
            break;
        }
    }

    /* Reset ModuleLLM */
    M5.Display.printf(">> Reset ModuleLLM..\n");
    module_llm.sys.reset();

    /* Setup LLM module and save returned work id */
    M5.Display.printf(">> Setup llm..\n");
    m5_module_llm::ApiLlmSetupConfig_t llm_config;
    llm_config.max_token_len = 1023;
    llm_work_id              = module_llm.llm.setup(llm_config);

    M5.Display.printf(">> Setup finish\n");
    M5.Display.printf(">> Try send your question via usb serial port\n");
    M5.Display.setTextColor(TFT_GREEN);
    M5.Display.printf("e.g. \nHi, What's your name?\n");
    M5.Display.printf("(end with CRLF \\r\\n)\n\n");
}

void loop()
{
    /* Check comm serial port and get received question */
    question_ok = false;
    if (CommSerialPort.available()) {
        while (CommSerialPort.available()) {
            char in_char = (char)CommSerialPort.read();
            received_question += in_char;

            /* Check if question finish */
            if (received_question.endsWith("\r\n")) {
                received_question.remove(received_question.length() - 2);
                question_ok = true;
                break;
            }
        }
    }

    /* If question is ready */
    if (question_ok) {
        M5.Display.setTextColor(TFT_GREEN);
        M5.Display.printf("<< %s\n", received_question.c_str());
        M5.Display.setTextColor(TFT_YELLOW);
        M5.Display.printf(">> ");
        CommSerialPort.printf("<< \"%s\"\n", received_question.c_str());
        CommSerialPort.print(">> ");

        /* Push question to LLM module and wait inference result */
        module_llm.llm.inferenceAndWaitResult(llm_work_id, received_question.c_str(), [](String& result) {
            /* Show result on screen and usb serial */
            M5.Display.printf("%s", result.c_str());
            CommSerialPort.print(result);
        });

        /* Clear for next question */
        received_question.clear();

        M5.Display.println();
        CommSerialPort.println();
    }

    delay(20);
}
