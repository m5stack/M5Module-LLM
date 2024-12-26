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
};

class ApiCamera {
public:
    void init(ModuleMsg* moduleMsg);

    /**
     * @brief Setup module camera, return work_id
     *
     * @param config
     * @param request_id
     * @return String
     */
    String setup(ApiCameraSetupConfig_t config = ApiCameraSetupConfig_t(), String request_id = "camera_setup");

private:
    ModuleMsg* _module_msg = nullptr;
};

}  // namespace m5_module_llm
