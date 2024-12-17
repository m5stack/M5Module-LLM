/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#include "../M5ModuleLLM.h"
#include "../src/api/api_version.h"

using namespace m5_module_llm;

#define _debug(format, ...) printf("[\033[1;32mVoiceAssistant\033[0m] " format "\n", ##__VA_ARGS__)

int M5ModuleLLM_VoiceAssistant::begin(String wakeUpKeyword, String prompt, String language)
{
    // Check connection
    if (!_m5_module_llm->checkConnection()) {
        return MODULE_LLM_WAIT_RESPONSE_TIMEOUT;
    }

    // Reset module
    _debug("reset M5ModuleLLM..");
    _m5_module_llm->sys.reset(true);

    _debug("setup module audio..");
    _work_id.audio = _m5_module_llm->audio.setup();
    if (_work_id.audio.isEmpty()) {
        return MODULE_LLM_ERROR_NONE;
    }

    _debug("setup module kws..");
    {
        ApiKwsSetupConfig_t config;
        config.kws   = wakeUpKeyword;
        _work_id.kws = _m5_module_llm->kws.setup(config, "kws_setup", language);
    }
    if (_work_id.kws.isEmpty()) {
        return MODULE_LLM_ERROR_NONE;
    }

    _debug("setup module asr..");
    {
        ApiAsrSetupConfig_t config;
        config.input = {"sys.pcm", _work_id.kws};
        _work_id.asr = _m5_module_llm->asr.setup(config, "asr_setup", language);
    }
    if (_work_id.asr.isEmpty()) {
        return MODULE_LLM_ERROR_NONE;
    }

    _debug("setup module llm..");
    {
        ApiLlmSetupConfig_t config;
        config.input  = {_work_id.asr, _work_id.kws};
        config.prompt = prompt;
        _work_id.llm  = _m5_module_llm->llm.setup(config);
    }
    if (_work_id.llm.isEmpty()) {
        return MODULE_LLM_ERROR_NONE;
    }

    _debug("setup module tts..");
    {
        if (!llm_version) {
            ApiTtsSetupConfig_t config;
            config.input = {_work_id.llm, _work_id.kws};
            _work_id.tts = _m5_module_llm->tts.setup(config, "tts_setup", language);
        } else {
            ApiMelottsSetupConfig_t config;
            config.input     = {_work_id.llm, _work_id.kws};
            _work_id.melotts = _m5_module_llm->melotts.setup(config, "tts_setup", language);
        }
    }
    if (_work_id.tts.isEmpty() && _work_id.melotts.isEmpty()) {
        return MODULE_LLM_ERROR_NONE;
    }

    return MODULE_LLM_OK;
}

void M5ModuleLLM_VoiceAssistant::update()
{
    // Update module
    _m5_module_llm->update();

    // Handle response msg
    for (auto& msg : _m5_module_llm->msg.responseMsgList) {
        // KWS msg
        if (_on_keyword_detected && (msg.work_id == _work_id.kws)) {
            _on_keyword_detected();
        }

        // ASR msg
        if (msg.work_id == _work_id.asr) {
            if (_on_asr_data_input_raw) {
                _on_asr_data_input_raw(msg.raw_msg);
            }
            if (_on_asr_data_input) {
                if (msg.object == "asr.utf-8.stream") {
                    deserializeJson(_json_doc, msg.raw_msg);
                    _on_asr_data_input(_json_doc["data"]["delta"].as<String>(), _json_doc["data"]["finish"],
                                       _json_doc["data"]["index"]);
                    _json_doc.clear();
                }
            }
        }

        // LLM msg
        if (msg.work_id == _work_id.llm) {
            if (_on_llm_data_input_raw) {
                _on_llm_data_input_raw(msg.raw_msg);
            }
            if (_on_llm_data_input) {
                if (msg.object == "llm.utf-8.stream") {
                    deserializeJson(_json_doc, msg.raw_msg);
                    _on_llm_data_input(_json_doc["data"]["delta"].as<String>(), _json_doc["data"]["finish"],
                                       _json_doc["data"]["index"]);
                    _json_doc.clear();
                }
            }
        }
    }

    _m5_module_llm->msg.responseMsgList.clear();
}

void M5ModuleLLM_VoiceAssistant::onKeywordDetected(OnKeywordDetectedCallback_t callback)
{
    _on_keyword_detected = callback;
}

void M5ModuleLLM_VoiceAssistant::onAsrDataInput(OnAsrDataInputCallback_t callback)
{
    _on_asr_data_input = callback;
}

void M5ModuleLLM_VoiceAssistant::onLlmDataInput(OnAsrDataInputCallback_t callback)
{
    _on_llm_data_input = callback;
}

void M5ModuleLLM_VoiceAssistant::onAsrDataInputRaw(OnAsrDataInputRawCallback_t callback)
{
    _on_asr_data_input_raw = callback;
}

void M5ModuleLLM_VoiceAssistant::onLlmDataInputRaw(OnLlmDataInputRawCallback_t callback)
{
    _on_llm_data_input_raw = callback;
}
