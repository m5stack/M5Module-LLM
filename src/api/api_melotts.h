/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once
#include "../utils/msg.h"
#include <Arduino.h>

namespace m5_module_llm {

struct ApiMelottsSetupConfig_t {
    String model              = "melotts_zh-cn";
    String response_format    = "sys.pcm";
    std::vector<String> input = {"tts.utf-8.stream"};
    bool enoutput             = false;
    bool enaudio              = true;
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

class ApiMelotts {
public:
    void init(ModuleMsg* moduleMsg);

    /**
     * @brief Setup module TTS, return TTS work_id
     *
     * @param config
     * @param request_id
     * @return String
     */
    String setup(ApiMelottsSetupConfig_t config = ApiMelottsSetupConfig_t(), String request_id = "melotts_setup",
                 String language = "en_US");

    /**
     * @brief Exit module TTS, return TTS work_id
     *
     * @param work_id
     * @param request_id
     * @return String
     */
    String exit(String work_id, String request_id = "tts_exit");

    /**
     * @brief Inference input data by TTS module
     *
     * @param work_id
     * @param input
     * @param timeout wait response timeout, default 0 (do not wait response)
     * @param request_id
     * @return int
     */
    int inference(String work_id, String input, uint32_t timeout = 0, String request_id = "tts_inference");

private:
    ModuleMsg* _module_msg = nullptr;
};

}  // namespace m5_module_llm
