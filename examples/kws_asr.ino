/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#include <Arduino.h>
#include <M5Unified.h>
#include <M5ModuleLLM.h>
#include <ArduinoJson.h>

M5ModuleLLM module_llm;
// Must be all uppercase letter
String wake_up_keyword = "HELLO";
String kws_work_id;
String asr_work_id;

void setup()
{
    M5.begin();
    M5.Display.setTextSize(2);
    M5.Display.setTextScroll(true);

    Serial2.begin(115200, SERIAL_8N1, 16, 17);  // Basic
    // Serial2.begin(115200, SERIAL_8N1, 13, 14);  // Core2
    // Serial2.begin(115200, SERIAL_8N1, 18, 17);  // CoreS3

    module_llm.begin(&Serial2);

    M5.Display.printf(">> Check ModuleLLM connection..\n");
    while (1) {
        if (module_llm.checkConnection()) {
            break;
        }
    }

    M5.Display.printf(">> Reset ModuleLLM..\n");
    module_llm.sys.reset();

    M5.Display.printf(">> Setup audio..\n");
    module_llm.audio.setup();

    M5.Display.printf(">> Setup kws..\n");
    m5_module_llm::ApiKwsSetupConfig_t kws_config;
    kws_config.kws = wake_up_keyword;
    kws_work_id    = module_llm.kws.setup(kws_config);

    M5.Display.printf(">> Setup asr..\n");
    asr_work_id = module_llm.asr.setup();

    M5.Display.printf(">> Setup ok\n>> Say \"%s\" to wakeup\n", wake_up_keyword.c_str());
}

void loop()
{
    module_llm.update();

    // Handle response msg
    for (auto& msg : module_llm.msg.responseMsgList) {
        // KWS msg
        if (msg.work_id == kws_work_id) {
            M5.Display.setTextColor(TFT_GREENYELLOW);
            M5.Display.printf(">> Keyword detected\n");
        }

        // ASR msg
        if (msg.work_id == asr_work_id) {
            if (msg.object == "asr.utf-8.stream") {
                // Parse and get asr result
                JsonDocument doc;
                deserializeJson(doc, msg.raw_msg);
                String asr_result = doc["data"]["delta"].as<String>();

                M5.Display.setTextColor(TFT_YELLOW);
                M5.Display.printf(">> %s\n", asr_result.c_str());
            }
        }
    }

    module_llm.msg.responseMsgList.clear();
}
