/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once
#include "../utils/msg.h"
#include <Arduino.h>

namespace m5_module_llm {

struct ApiVadSetupConfig_t {
    String model              = "silero-vad";
    String response_format    = "vad.bool";
    std::vector<String> input = {"sys.pcm"};
    bool enoutput             = true;
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

class ApiVad {
public:
    void init(ModuleMsg* moduleMsg);

    /**
     * @brief Setup module VAD, return VAD work_id
     *
     * @param config
     * @param request_id
     * @return String
     */
    String setup(ApiVadSetupConfig_t config = ApiVadSetupConfig_t(), String request_id = "vad_setup");

    /**
     * @brief Exit module VAD, return VAD work_id
     *
     * @param work_id
     * @param request_id
     * @return String
     */
    String exit(String work_id, String request_id = "vad_exit");

private:
    ModuleMsg* _module_msg = nullptr;
};

}  // namespace m5_module_llm
