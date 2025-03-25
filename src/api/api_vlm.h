/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once
#include "../utils/msg.h"
#include <Arduino.h>

namespace m5_module_llm {

struct ApiVlmSetupConfig_t {
    String prompt;
    String model              = "internvl2.5-1B-ax630c";
    String response_format    = "vlm.utf-8.stream";
    std::vector<String> input = {"vlm.utf-8.stream"};
    bool enoutput             = true;
    bool enkws                = true;
    // int max_token_len         = 127;
    int max_token_len = 255;
};

class ApiVlm {
public:
    void init(ModuleMsg* moduleMsg);

    /**
     * @brief Setup module VLLM, return VLLM work_id
     *
     * @param config
     * @param request_id
     * @return String
     */
    String setup(ApiVlmSetupConfig_t config = ApiVlmSetupConfig_t(), String request_id = "vlm_setup");

    /**
     * @brief Exit module VLLM, return VLLM work_id
     *
     * @param work_id
     * @param request_id
     * @return String
     */
    String exit(String work_id, String request_id = "vlm_exit");

    /**
     * @brief Inference input data by module VLLM
     *
     * @param work_id
     * @param input
     * @param request_id
     * @return int
     */
    int inference(String work_id, String input, String request_id = "vlm_inference");

    /**
     * @brief Inference input data by module VLLM, and wait inference result
     *
     * @param work_id
     * @param input
     * @param onResult On inference result callback
     * @param timeout
     * @param request_id
     * @return int
     */
    int inferenceAndWaitResult(String work_id, String input, std::function<void(String&)> onResult,
                               uint32_t timeout = 5000, String request_id = "vlm_inference");

private:
    ModuleMsg* _module_msg = nullptr;
};

}  // namespace m5_module_llm
