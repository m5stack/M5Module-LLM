/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once
#include "../utils/msg.h"
#include <Arduino.h>

namespace m5_module_llm {

struct ApiCameraSetupConfig_t {
    String response_format = "camera.raw";
    String input           = "/dev/video0";
    bool enoutput          = false;
    int frame_width        = 320;
    int frame_height       = 320;
    std::map<String, String> extra_params;

    template <typename T>
    void setParam(const String& key, T value)
    {
        extra_params[key] = String(value);
    }

    void setParam(const String& key, bool value)
    {
        extra_params[key] = value ? "bool_true" : "bool_false";
    }

    void setParam(const String& key, float value)
    {
        char buffer[16];
        dtostrf(value, 1, 6, buffer);
        extra_params[key] = String(buffer);
    }
};

class ApiCamera {
public:
    void init(ModuleMsg* moduleMsg);

    /**
     * @brief Setup module camera, return camera work_id
     *
     * @param config
     * @param request_id
     * @return String
     */
    String setup(ApiCameraSetupConfig_t config = ApiCameraSetupConfig_t(), String request_id = "camera_setup");

    /**
     * @brief Exit module camera, return camera work_id
     *
     * @param work_id
     * @param request_id
     * @return String
     */
    String exit(String work_id, String request_id = "camera_exit");

private:
    ModuleMsg* _module_msg = nullptr;
};

}  // namespace m5_module_llm
