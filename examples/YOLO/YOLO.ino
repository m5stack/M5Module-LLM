/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#include <Arduino.h>
#include <M5Unified.h>
#include <M5ModuleLLM.h>

M5ModuleLLM module_llm;
String camera_work_id;
String yolo_work_id;

void clearDisplay()
{
    M5.Display.fillRect(40, 50, 270, 20, BLACK);
    M5.Display.fillRect(150, 80, 60, 20, BLACK);
    M5.Display.fillRect(40, 110, 40, 20, BLACK);
    M5.Display.fillRect(40, 140, 40, 20, BLACK);
    M5.Display.fillRect(40, 170, 40, 20, BLACK);
    M5.Display.fillRect(40, 200, 40, 20, BLACK);
}

void setup()
{
    M5.begin();
    M5.Display.setTextSize(2);
    M5.Display.setTextScroll(true);

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
    M5.Display.setTextColor(ORANGE, BLACK);
    M5.Display.setTextSize(2);
    M5.Display.setTextDatum(middle_center);
    M5.Display.drawString("Check ModuleLLM connection..", M5.Display.width() / 2, M5.Display.height() / 2);
    while (1) {
        if (module_llm.checkConnection()) {
            break;
        }
    }

    /* Reset ModuleLLM */
    M5.Display.fillRect(0, (M5.Display.height() / 2) - 10, 320, 25, BLACK);
    M5.Display.drawString("Reset ModuleLLM..", M5.Display.width() / 2, M5.Display.height() / 2);
    module_llm.sys.reset();

    /* Setup Camera module */
    M5.Display.fillRect(0, (M5.Display.height() / 2) - 10, 320, 25, BLACK);
    M5.Display.drawString("Setup camera..", M5.Display.width() / 2, M5.Display.height() / 2);
    camera_work_id = module_llm.camera.setup();

    /* Setup YOLO module and save returned work id */
    M5.Display.fillRect(0, (M5.Display.height() / 2) - 10, 320, 25, BLACK);
    M5.Display.drawString("Setup yolo..", M5.Display.width() / 2, M5.Display.height() / 2);
    m5_module_llm::ApiYoloSetupConfig_t yolo_config;
    yolo_config.input = {camera_work_id};
    yolo_work_id      = module_llm.yolo.setup(yolo_config, "yolo_setup");

    M5.Display.fillRect(0, (M5.Display.height() / 2) - 10, 320, 25, BLACK);

    M5.Display.setTextDatum(top_left);
    M5.Display.drawString("class", 10, 20);
    M5.Display.drawString("confidence", 10, 80);
    M5.Display.drawString("x1", 10, 110);
    M5.Display.drawString("y1", 10, 140);
    M5.Display.drawString("x2", 10, 170);
    M5.Display.drawString("y2", 10, 200);
}

void loop()
{
    /* Update ModuleLLM */
    module_llm.update();

    /* Handle module response messages */
    for (auto& msg : module_llm.msg.responseMsgList) {
        /* If YOLO module message */
        if (msg.work_id == yolo_work_id) {
            /* Check message object type */
            if (msg.object == "yolo.box.stream") {
                /* Parse message json and get YOLO result */
                JsonDocument doc;
                deserializeJson(doc, msg.raw_msg);
                JsonArray delta = doc["data"]["delta"].as<JsonArray>();

                if (delta.size() > 0) {
                    JsonObject result   = delta[0].as<JsonObject>();
                    String class_name   = result["class"].as<String>();
                    float confidence    = result["confidence"].as<float>();
                    JsonArray bboxArray = result["bbox"].as<JsonArray>();

                    if (bboxArray.size() == 4) {
                        int x1 = bboxArray[0].as<int>();
                        int y1 = bboxArray[1].as<int>();
                        int x2 = bboxArray[2].as<int>();
                        int y2 = bboxArray[3].as<int>();

                        clearDisplay();

                        M5.Display.drawString(class_name, 40, 50);
                        M5.Display.drawFloat(confidence, 2, 150, 80);
                        M5.Display.drawNumber(x1, 40, 110);
                        M5.Display.drawNumber(y1, 40, 140);
                        M5.Display.drawNumber(x2, 40, 170);
                        M5.Display.drawNumber(y2, 40, 200);
                    }
                } else {
                    clearDisplay();
                }
            }
        }
    }

    /* Clear handled messages */
    module_llm.msg.responseMsgList.clear();
}