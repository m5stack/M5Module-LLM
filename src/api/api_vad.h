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
