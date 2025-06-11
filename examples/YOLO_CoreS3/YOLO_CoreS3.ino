/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#include <Arduino.h>
#include <M5Unified.h>
#include <M5ModuleLLM.h>
#include <M5GFX.h>

#include "M5CoreS3.h"

M5ModuleLLM module_llm;
String yolo_work_id;

struct DetectionResult {
    String class_name;
    float confidence;
    int x1;
    int y1;
    int x2;
    int y2;
};

M5Canvas canvas(&M5.Display);

void setup()
{
    M5.begin();
    M5.Display.setTextSize(2);
    M5.Display.setTextScroll(true);

    canvas.createSprite(M5.Display.width(), M5.Display.height());

    /* Init M5CoreS3 Camera */
    CoreS3.Camera.begin();
    CoreS3.Camera.sensor->set_framesize(CoreS3.Camera.sensor, FRAMESIZE_QVGA);

    /* Init module serial port */
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

    /* Set ModuleLLM baud rate */
    M5.Display.printf(">> ModuleLLM connected, set baud rate to 1500000\n");
    module_llm.setBaudRate(1500000);

    Serial2.begin(1500000, SERIAL_8N1, rxd, txd);
    module_llm.begin(&Serial2);

    /* Setup YOLO module and save returned work id */
    M5.Display.printf(">> Setup yolo..\n");
    yolo_work_id = module_llm.yolo.setup();
    canvas.setFont(&fonts::FreeSerifBold12pt7b);
}

DetectionResult parseDetection(String& jsonStr)
{
    DetectionResult detection;
    JsonDocument doc;
    deserializeJson(doc, jsonStr);
    JsonObject obj = doc.as<JsonObject>();
    if (obj["bbox"].is<JsonArray>() && obj["class"].is<const char*>() && obj["confidence"].is<const char*>()) {
        detection.class_name = obj["class"].as<const char*>();
        detection.confidence = atof(obj["confidence"].as<const char*>());
        JsonArray bbox       = obj["bbox"].as<JsonArray>();
        if (bbox.size() == 4) {
            detection.x1 = (int)atof(bbox[0].as<const char*>());
            detection.y1 = (int)atof(bbox[1].as<const char*>());
            detection.x2 = (int)atof(bbox[2].as<const char*>());
            detection.y2 = (int)atof(bbox[3].as<const char*>());
        }
    }
    return detection;
}

void loop()
{
    if (CoreS3.Camera.get()) {
        uint8_t* out_jpg   = NULL;
        size_t out_jpg_len = 0;
        frame2jpg(CoreS3.Camera.fb, 50, &out_jpg, &out_jpg_len);
        canvas.pushImage(0, 0, CoreS3.Display.width(), CoreS3.Display.height(), (uint16_t*)CoreS3.Camera.fb->buf);
        module_llm.yolo.inferenceAndWaitResult(
            yolo_work_id, out_jpg, out_jpg_len,
            [](String& result) {
                DetectionResult detection = parseDetection(result);
                int y1_pos                = detection.y1 - 40;
                if (y1_pos < 24) y1_pos = 24;
                String combinedResult = detection.class_name + " " + String(detection.confidence, 2);
                canvas.drawString(combinedResult, detection.x1, y1_pos);
                canvas.drawRect(detection.x1, detection.y1 - 40, detection.x2, detection.y2 - 40, ORANGE);
            },
            10);
        canvas.pushSprite(0, 0);
        free(out_jpg);
    }
    CoreS3.Camera.free();
}