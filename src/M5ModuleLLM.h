/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once
#include <Arduino.h>
#include "utils/comm.h"
#include "utils/msg.h"
#include "api/api_sys.h"
#include "api/api_llm.h"
#include "api/api_audio.h"
#include "api/api_tts.h"
#include "api/api_kws.h"
#include "api/api_asr.h"

class M5ModuleLLM {
public:
    bool begin(Stream* targetPort);
    bool checkConnection();
    void update();

    m5_module_llm::ApiSys sys;
    m5_module_llm::ApiLlm llm;
    m5_module_llm::ApiAudio audio;
    m5_module_llm::ApiTts tts;
    m5_module_llm::ApiKws kws;
    m5_module_llm::ApiAsr asr;
    m5_module_llm::ModuleMsg msg;
    m5_module_llm::ModuleComm comm;

private:
};

typedef std::function<void(void)> OnKeywordDetectedCallback_t;
typedef std::function<void(String data, bool isFinish, int index)> OnAsrDataInputCallback_t;
typedef std::function<void(String data, bool isFinish, int index)> OnLlmDataInputCallback_t;
typedef std::function<void(String rawData)> OnAsrDataInputRawCallback_t;
typedef std::function<void(String rawData)> OnLlmDataInputRawCallback_t;

class M5ModuleLLM_VoiceAssistant {
public:
    M5ModuleLLM_VoiceAssistant(M5ModuleLLM* m5ModuleLlm) : _m5_module_llm(m5ModuleLlm)
    {
    }

    int begin(String wakeUpKeyword = "HELLO", String prompt = "");

    void update();

    void onKeywordDetected(OnKeywordDetectedCallback_t callback);
    void onAsrDataInput(OnAsrDataInputCallback_t callback);
    void onLlmDataInput(OnLlmDataInputCallback_t callback);

    void onAsrDataInputRaw(OnAsrDataInputRawCallback_t callback);
    void onLlmDataInputRaw(OnLlmDataInputRawCallback_t callback);

private:
    struct WorkId_t {
        String audio;
        String kws;
        String asr;
        String llm;
        String tts;
    };

    WorkId_t _work_id;
    M5ModuleLLM* _m5_module_llm;
    JsonDocument _json_doc;
    OnKeywordDetectedCallback_t _on_keyword_detected   = nullptr;
    OnAsrDataInputCallback_t _on_asr_data_input        = nullptr;
    OnLlmDataInputCallback_t _on_llm_data_input        = nullptr;
    OnAsrDataInputRawCallback_t _on_asr_data_input_raw = nullptr;
    OnLlmDataInputRawCallback_t _on_llm_data_input_raw = nullptr;
};
