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
    std::vector<String> input = {"sys.pcm"};
    bool enoutput             = false;
    bool enaudio              = true;
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
    String setup(ApiMelottsSetupConfig_t config = ApiMelottsSetupConfig_t(), String request_id = "tts_setup");

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
