/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once
#include "../utils/msg.h"
#include <Arduino.h>

namespace m5_module_llm {

struct ApiAsrSetupConfig_t {
    String model              = "sherpa-ncnn-streaming-zipformer-20M-2023-02-17";
    String response_format    = "asr.utf-8.stream";
    std::vector<String> input = {"sys.pcm"};
    bool enoutput             = true;
    bool enkws                = true;
    float rule1               = 2.4;
    float rule2               = 1.2;
    float rule3               = 30.0;
};

class ApiAsr {
public:
    void init(ModuleMsg* moduleMsg);

    /**
     * @brief Setup module ASR, return ASR work_id
     *
     * @param config
     * @param request_id
     * @return String
     */
    String setup(ApiAsrSetupConfig_t config = ApiAsrSetupConfig_t(), String request_id = "asr_setup", String language = "en_US");

private:
    ModuleMsg* _module_msg = nullptr;
};

}  // namespace m5_module_llm
