/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once
#include "../utils/msg.h"
#include <Arduino.h>

namespace m5_module_llm {

struct ApiLlmSetupConfig_t {
    String prompt;
    String model           = "qwen2.5-0.5b";
    String response_format = "llm.utf-8.stream";
    String input           = "llm.utf-8.stream";
    bool enoutput          = true;
    bool enkws             = true;
    int max_token_len      = 127;
};

class ApiLlm {
public:
    void init(ModuleMsg* moduleMsg);

    /**
     * @brief Setup module llm, return work_id
     *
     * @param config
     * @param request_id
     * @return String
     */
    String setup(ApiLlmSetupConfig_t config = ApiLlmSetupConfig_t(), String request_id = "llm_setup");

    int inference(String work_id, String input, String request_id = "llm_inference");

    int inferenceAndWaitResult(String work_id, String input, std::function<void(String&)> onResult,
                               uint32_t timeout = 5000, String request_id = "llm_inference");

private:
    ModuleMsg* _module_msg = nullptr;
};

}  // namespace m5_module_llm
