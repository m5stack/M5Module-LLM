/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once
#include "../utils/msg.h"
#include <Arduino.h>

namespace m5_module_llm {

struct ApiWhisperSetupConfig_t {
    String model              = "whisper-tiny";
    String response_format    = "asr.utf-8";
    String language           = "en";
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

class ApiWhisper {
public:
    void init(ModuleMsg* moduleMsg);

    /**
     * @brief Setup module ASR, return ASR work_id
     *
     * @param config
     * @param request_id
     * @return String
     */
    String setup(ApiWhisperSetupConfig_t config = ApiWhisperSetupConfig_t(), String request_id = "asr_setup",
                 String language = "en_US");

    /**
     * @brief Exit module ASR, return ASR work_id
     *
     * @param work_id
     * @param request_id
     * @return String
     */
    String exit(String work_id, String request_id = "asr_exit");

private:
    ModuleMsg* _module_msg = nullptr;
};

}  // namespace m5_module_llm
