/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once
#include "../utils/msg.h"
#include <Arduino.h>

namespace m5_module_llm {

struct ApiTtsSetupConfig_t {
    String model           = "single_speaker_english_fast";
    String response_format = "tts.base64.wav";
    String input           = "tts.utf-8.stream";
    bool enoutput          = true;
    bool enkws             = true;
};

class ApiTts {
public:
    void init(ModuleMsg* moduleMsg);

    /**
     * @brief Setup module TTS, return TTS work_id
     *
     * @param config
     * @param request_id
     * @return String
     */
    String setup(ApiTtsSetupConfig_t config = ApiTtsSetupConfig_t(), String request_id = "tts_setup");

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
