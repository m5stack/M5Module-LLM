/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once
#include "../utils/msg.h"
#include <Arduino.h>

namespace m5_module_llm {

struct ApiAudioSetupConfig_t {
    int capcard      = 0;
    int capdevice    = 0;
    float capVolume  = 0.5;
    int playcard     = 0;
    int playdevice   = 1;
    float playVolume = 0.15;
};

class ApiAudio {
public:
    void init(ModuleMsg* moduleMsg);

    /**
     * @brief Setup module audio, return work_id
     *
     * @param config
     * @param request_id
     * @return String
     */
    String setup(ApiAudioSetupConfig_t config = ApiAudioSetupConfig_t(), String request_id = "audio_setup");

private:
    ModuleMsg* _module_msg = nullptr;
};

}  // namespace m5_module_llm
