/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once
#include "../utils/msg.h"
#include <Arduino.h>

namespace m5_module_llm {
struct ApiDepthAnythingSetupConfig_t {
    String model              = "depth_anything";
    String response_format    = "jpeg.base64.stream";
    std::vector<String> input = {"depth_anything.jpeg.raw"};
    bool enoutput             = true;
};

class ApiDepthAnything {
public:
    void init(ModuleMsg* moduleMsg);

    /**
     * @brief Setup module DepthAnything, return DepthAnything work_id
     *
     * @param config
     * @param request_id
     * @return String
     */
    String setup(ApiDepthAnythingSetupConfig_t config = ApiDepthAnythingSetupConfig_t(),
                 String request_id                    = "depth_anything_setup");

    /**
     * @brief Exit module DepthAnything, return DepthAnything work_id
     *
     * @param work_id
     * @param request_id
     * @return String
     */
    String exit(String work_id, String request_id = "depth_anything_exit");

    /**
     * @brief Inference input data by module LLM
     *
     * @param raw_len
     * @param work_id
     * @param input
     * @param request_id
     * @return int
     */
    int inference(String& work_id, uint8_t* input, size_t& raw_len, String request_id = "depth_anything_inference");

    /**
     * @brief Inference input data by module LLM, and wait inference result
     *
     * @param raw_len
     * @param work_id
     * @param input
     * @param onResult On inference result callback
     * @param timeout
     * @param request_id
     * @return int
     */
    int inferenceAndWaitResult(String& work_id, uint8_t* input, size_t& raw_len, std::function<void(String&)> onResult,
                               uint32_t timeout = 5000, String request_id = "depth_anything_inference");

private:
    ModuleMsg* _module_msg = nullptr;
};
}  // namespace m5_module_llm
