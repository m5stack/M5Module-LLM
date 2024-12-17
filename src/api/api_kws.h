/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once
#include "../utils/msg.h"
#include <Arduino.h>

namespace m5_module_llm {

struct ApiKwsSetupConfig_t {
    String kws                = "HELLO";
    String model              = "sherpa-onnx-kws-zipformer-gigaspeech-3.3M-2024-01-01";
    String response_format    = "kws.bool";
    std::vector<String> input = {"sys.pcm"};
    bool enoutput             = true;
};

class ApiKws {
public:
    void init(ModuleMsg* moduleMsg);

    /**
     * @brief Setup module KWS, return KWS work_id
     *
     * @param config
     * @param request_id
     * @return String
     */
    String setup(ApiKwsSetupConfig_t config = ApiKwsSetupConfig_t(), String request_id = "kws_setup", String language = "en_US");

private:
    ModuleMsg* _module_msg = nullptr;
};

}  // namespace m5_module_llm
