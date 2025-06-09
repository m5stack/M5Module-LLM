/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#include <Arduino.h>
#include <M5Unified.h>
#include <M5ModuleLLM.h>

M5ModuleLLM module_llm;
String melotts_work_id;
String language;

void setup()
{
    M5.begin();
    M5.Display.setTextSize(2);
    M5.Display.setTextScroll(true);
    // M5.Display.setFont(&fonts::efontCN_12);  // Support Chinese display
    // M5.Display.setFont(&fonts::efontJA_12);  // Support Japanese display

    language = "en_US";
    // language = "zh_CN";
    // language = "ja_JP";

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

    /* Setup Audio module */
    M5.Display.printf(">> Setup audio..\n");
    module_llm.audio.setup();

    /* Setup MeloTTS module and save returned work id */
    M5.Display.printf(">> Setup melotts..\n\n");
    m5_module_llm::ApiMelottsSetupConfig_t melotts_config;
    melotts_work_id = module_llm.melotts.setup(melotts_config, "melotts_setup", language);
}

void loop()
{
    /* Make a text for speech: {i} plus {i} equals to {i + i} */
    static int i = 0;
    i++;
    std::string text = std::to_string(i) + " plus " + std::to_string(i) + " equals " + std::to_string(i + i) + ".";
    // std::string text = std::to_string(i) + " 加 " + std::to_string(i) + " 等于 " + std::to_string(i + i) + ".";

    M5.Display.setTextColor(TFT_GREEN);
    M5.Display.printf("<< %s\n\n", text.c_str());

    /* Push text to TTS module and wait inference result */
    module_llm.tts.inference(melotts_work_id, text.c_str(), 10000);

    delay(500);
}