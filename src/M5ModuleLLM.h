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
#include "api/api_melotts.h"
#include "api/api_kws.h"
#include "api/api_asr.h"
#include "api/api_yolo.h"

class M5ModuleLLM {
public:
    /**
     * @brief Init module
     *
     * @param targetPort module serial port
     * @return true
     * @return false
     */
    bool begin(Stream* targetPort);

    /**
     * @brief Check if module is connected
     *
     * @return true
     * @return false
     */
    bool checkConnection();

    /**
     * @brief Update module
     *
     */
    void update();

    /**
     * @brief SYS module api set
     *
     */
    m5_module_llm::ApiSys sys;

    /**
     * @brief LLM module api set
     *
     */
    m5_module_llm::ApiLlm llm;

    /**
     * @brief Audio module api set
     *
     */
    m5_module_llm::ApiAudio audio;

    /**
     * @brief TTS module api set
     *
     */
    m5_module_llm::ApiTts tts;

    /**
     * @brief MELOTTS module api set
     *
     */
    m5_module_llm::ApiMelotts melotts;

    /**
     * @brief KWS module api set
     *
     */
    m5_module_llm::ApiKws kws;

    /**
     * @brief ASR module api set
     *
     */
    m5_module_llm::ApiAsr asr;

    /**
     * @brief YOLO module api set
     *
     */
    m5_module_llm::ApiYolo yolo;

    /**
     * @brief MSG module to handle module response message
     *
     */
    m5_module_llm::ModuleMsg msg;

    /**
     * @brief COMM module to handle module communication stream
     *
     */
    m5_module_llm::ModuleComm comm;

private:
};

typedef std::function<void(void)> OnKeywordDetectedCallback_t;
typedef std::function<void(String data, bool isFinish, int index)> OnAsrDataInputCallback_t;
typedef std::function<void(String data, bool isFinish, int index)> OnLlmDataInputCallback_t;
typedef std::function<void(String data, bool isFinish, int index)> OnYoloDataInputCallback_t;
typedef std::function<void(String rawData)> OnAsrDataInputRawCallback_t;
typedef std::function<void(String rawData)> OnLlmDataInputRawCallback_t;
typedef std::function<void(String rawData)> OnYoloDataInputRawCallback_t;

/**
 * @brief Voice assistant preset base on class M5ModuleLLM
 *
 */
class M5ModuleLLM_VoiceAssistant {
public:
    M5ModuleLLM_VoiceAssistant(M5ModuleLLM* m5ModuleLlm) : _m5_module_llm(m5ModuleLlm)
    {
    }

    /**
     * @brief Start voice assistant preset
     *
     * @param wakeUpKeyword
     * @param prompt
     * @return int
     */
    int begin(String wakeUpKeyword = "HELLO", String prompt = "");

    /**
     * @brief Update voice assistant preset, trigger callbacks
     *
     */
    void update();

    /**
     * @brief Register on key-word detected callback
     *
     * @param callback
     */
    void onKeywordDetected(OnKeywordDetectedCallback_t callback);

    /**
     * @brief Register on ASR data input callback
     *
     * @param callback
     */
    void onAsrDataInput(OnAsrDataInputCallback_t callback);

    /**
     * @brief Register on LLM data input callback
     *
     * @param callback
     */
    void onLlmDataInput(OnLlmDataInputCallback_t callback);

    /**
     * @brief Register on ASR raw data (module response json) input callback
     *
     * @param callback
     */
    void onAsrDataInputRaw(OnAsrDataInputRawCallback_t callback);

    /**
     * @brief Register on LLM raw data (module response json) input callback
     *
     * @param callback
     */
    void onLlmDataInputRaw(OnLlmDataInputRawCallback_t callback);

private:
    struct WorkId_t {
        String audio;
        String kws;
        String asr;
        String llm;
        String tts;
        String melotts;
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
