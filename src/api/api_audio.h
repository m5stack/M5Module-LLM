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

class ApiAudio {
public:
    void init(ModuleMsg* moduleMsg);

    /**
     * @brief Setup module audio, return audio work_id
     *
     * @param config
     * @param request_id
     * @return String
     */
    String setup(ApiAudioSetupConfig_t config = ApiAudioSetupConfig_t(), String request_id = "audio_setup");

    /**
     * @brief Exit module audio, return audio work_id
     *
     * @param work_id
     * @param request_id
     * @return String
     */
    String exit(String work_id, String request_id = "audio_exit");

private:
    ModuleMsg* _module_msg = nullptr;
};

}  // namespace m5_module_llm
