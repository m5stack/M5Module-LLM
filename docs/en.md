# M5Module-LLM Arduino API

[M5Module-LLM](https://github.com/m5stack/M5Module-LLM) Arduino Driver Library API Documentation.

## M5ModuleLLM Class

`M5ModuleLLM` is used to initialize the LLM Module and provides internal members for quick initialization of various LLM units, making it easier to build applications according to your needs.

```cpp
class M5ModuleLLM {
public:
    bool begin(Stream * targetPort);
    bool checkConnection();
    void update();

    m5_module_llm::ApiSys sys;
    m5_module_llm::ApiLlm llm;
    m5_module_llm::ApiAudio audio;
    m5_module_llm::ApiTts tts;
    m5_module_llm::ApiTts melotts;
    m5_module_llm::ApiKws kws;
    m5_module_llm::ApiAsr asr;
    m5_module_llm::ApiAsr yolo;
    m5_module_llm::ApiVad vad;
    m5_module_llm::ApiWhisper whisper;
    m5_module_llm::ApiDepthAnything depthanything;
    m5_module_llm::ModuleMsg msg;
    m5_module_llm::ModuleComm comm;
private:
};
```

### begin

**Function Prototype:**

```cpp
bool begin(Stream* targetPort);
```

**Function Description:**

- Initializes the LLM Module UART interface configuration.

**Parameters:**

- Stream\* targetPort:
    - Pass the Serial pointer.

**Return Value:**

- bool:
    - true: Initialization successful
    - false: Initialization failed

### checkConnection

**Function Prototype:**

```cpp
bool checkConnection();
```

**Function Description:**

- Sends the `sys.ping` command to check the connection status of the LLM Module.

**Parameters:**

- None

**Return Value:**

- bool:
    - true: Module responds
    - false: No response from module

### update

**Function Prototype:**

```cpp
void update();
```

**Function Description:**

- Pulls the LLM Module UART response data, this API should be included in the Loop and executed continuously.

**Parameters:**

- None

**Return Value:**

- None

## ApiSys Class

The internal member `ApiSys sys` of `M5ModuleLLM` is used to control the SYS unit, enabling operations like system reset.

### ping

**Function Prototype:**

```cpp
int ping();
```

**Function Description:**

- Sends the `sys.ping` command to check the connection status of the LLM Module.

**Parameters:**

- None

**Return Value:**

- int:
    - MODULE_LLM_OK / Error Code

### reset

**Function Prototype:**

```cpp
int reset(bool waitResetFinish = true);
```

**Function Description:**

- Sends the `sys.reset` command to reset the software service.

**Parameters:**

- bool waitResetFinish:
    - true: Blocks and waits for reset to finish
    - false: Performs reset without blocking

**Return Value:**

- int:
    - MODULE_LLM_OK / Error Code

### reboot

**Function Prototype:**

```cpp
int reboot();
```

**Function Description:**

- Sends the `sys.reboot` command to reboot the system.

**Parameters:**

- None

**Return Value:**

- int:
    - MODULE_LLM_OK / Error Code

## ApiAudio Class

Note: This function has been deprecated in version 1.3 and later, and is now automatically configured internally.

The internal member `ApiAudio audio` of `M5ModuleLLM` is used to control the initialization and configuration of the Audio unit.

### setup

**Function prototype:**

```cpp
String setup(ApiAudioSetupConfig_t config = ApiAudioSetupConfig_t(), String request_id = "audio_setup");
```

**Function description:**

- Initializes the Audio unit and activates the system sound card. (This feature must be enabled before using KWS and TTS)

**Parameters:**

ApiAudioSetupConfig_t config:

- LLM unit initialization configuration:
- String request_id:
    - Session ID, default can be used.

```cpp
struct ApiAudioSetupConfig_t {
    int capcard      = 0;
    int capdevice    = 0;
    float capVolume  = 0.5;
    int playcard     = 0;
    int playdevice   = 1;
    float playVolume = 0.15;
};
```

| Parameter   | Description       | Input Values                          |
| ----------- | ----------------- | ------------------------------------- |
| capcard     | Microphone sound card index | Default sound card: 0           |
| capdevice   | Microphone device index   | Onboard silicon microphone: 0    |
| capVolume   | Input volume      | 0.0～10.0 (1<volume increases gain, default is 0.5) |
| playcard    | Speaker sound card index | Default sound card: 0           |
| playdevice  | Speaker device index   | Onboard speaker: 1                |
| playVolume  | Output volume      | 0.0～10.0 (1<volume increases gain, default is 0.5) |

**Return Value:**

- String:
    - audio_work_id: audio unit work_id

## ApiCamera Class

The internal member `ApiCamera camera` of `M5ModuleLLM` is used to control the initialization and configuration of the Camera unit.

### setup

**Function prototype:**

```cpp
String setup(ApiCameraSetupConfig_t config = ApiCameraSetupConfig_t(), String request_id = "camera_setup");
```

**Function description:**

- Initializes the Camera unit and activates camera input. (This feature must be enabled before using UVC)

**Parameters:**

ApiCameraSetupConfig_t config:

- Camera unit initialization configuration:
- String request_id:
    - Session ID, default can be used.

```cpp
struct ApiCameraSetupConfig_t {
    String response_format = "camera.raw";
    String input           = "/dev/video0";
    bool enoutput          = false;
    int frame_width        = 320;
    int frame_height       = 320;
};
```

| Parameter     | Description             | Input Values                |
| ------------- | ----------------------- | --------------------------- |
| input         | UVC index               | "/dev/video0"               |
| enoutput      | Whether to output image data via serial | Enable: true<br>Disable: false |
| frame_width   | Image width             | 320                         |
| frame_height  | Image height            | 320                         |

**Return Value:**

- String:
    - camera_work_id: camera unit work_id

## ApiKws Class

The internal member `ApiKws kws` of `M5ModuleLLM` is used to control the initialization and configuration of the KWS unit.

### setup

**Function Prototype:**

```cpp
String setup(ApiKwsSetupConfig_t config = ApiKwsSetupConfig_t(), String request_id = "kws_setup",
             String language = "en_US");```

**Function Description:**

- Initializes the KWS unit and configures the wake-up keyword.

**Parameters:**

ApiKwsSetupConfig_t config:

- KWS unit initialization configuration:
- String request_id:
    - Session ID, default can be used.

```cpp
struct ApiKwsSetupConfig_t {
    String kws             = "HELLO";
    String model           = "sherpa-onnx-kws-zipformer-gigaspeech-3.3M-2024-01-01";
    String response_format = "kws.bool";
    String input           = "sys.pcm";
    bool enoutput          = true;
};
```

| Parameter | Description         | Input Values                                                                                                                             |
| --------- | ------------------- | ----------------------------------------------------------------------------------------------------------------------------------------- |
| model     | Conversion Model    | English Model: "sherpa-onnx-kws-zipformer-gigaspeech-3.3M-2024-01-01"<br>Chinese Model: "sherpa-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01" |
| kws       | KWS Wake-up Word Text | No mixing of Chinese and English; English must be in uppercase                                                                                             |
| enoutput  | Enable UART Output  | Enable: true<br>Disable: false                                                                                                            |

**Return Value:**

- String:
    - kws_work_id: KWS unit work_id

## ApiVad Class

The internal member `ApiVad vad` of `M5ModuleLLM` is used to control the initialization and configuration of the VAD unit.

### setup

**Function Prototype:**

```cpp
String setup(ApiVadSetupConfig_t config = ApiVadSetupConfig_t(), String request_id = "vad_setup");
```

**Function Description:**

- Initializes the VAD unit.

**Parameters:**

ApiVadSetupConfig_t config:

- VAD unit initialization configuration:
- String request_id:
    - Session ID, default can be used.

```cpp
struct ApiKwsSetupConfig_t {
    String model           = "silero-vad";
    String response_format = "vad.bool";
    String input           = {"sys.pcm", "kws.1000"};
    bool enoutput          = true;
};
```

| Parameter | Description      | Input Values                                                                                                          |
| --------- | --------------- | --------------------------------------------------------------------------------------------------------------------- |
| model     | Conversion Model | Model: "silero-vad"<br>                                                                                               |
| input     | Input            | KWS Wake-up Input: "kws.xxx" (input the KWS unit's work_id)<br>Onboard Microphone Input: "sys.pcm"<br>UART Stream Input: "vad.wav.stream.base64" |
| enoutput  | Enable UART Output | Enable: true<br>Disable: false                                                                                         |

**Return Value:**

- String:
    - vad_work_id: VAD unit work_id

## ApiAsr Class

The internal member `ApiAsr asr` of `M5ModuleLLM` is used to control the initialization and configuration of the ASR unit.

### setup

**Function Prototype:**

```cpp
String setup(ApiAsrSetupConfig_t config = ApiAsrSetupConfig_t(), String request_id = "asr_setup",
             String language = "en_US");
```

**Function Description:**

- Initializes the ASR unit and enables speech-to-text functionality.

**Input Parameters:**

ApiAsrSetupConfig_t config:

- ASR unit initialization configuration:
- String request_id:
    - Session ID, the default can be used.

```cpp
struct ApiAsrSetupConfig_t {
    String model           = "sherpa-ncnn-streaming-zipformer-20M-2023-02-17";
    String response_format = "asr.utf-8.stream";
    String input           = ["sys.pcm", "kws.1000"];
    bool enoutput          = true;
    float rule1            = 2.4;
    float rule2            = 1.2;
    float rule3            = 30.0;
};
```

| Parameter       | Description                          | Input Values                                                                                                                |
| --------------- | ------------------------------------ | --------------------------------------------------------------------------------------------------------------------------- |
| model           | Conversion model                     | English Model: "sherpa-ncnn-streaming-zipformer-20M-2023-02-17"<br>Chinese Model: "sherpa-ncnn-streaming-zipformer-zh-14M-2023-02-23" |
| response_format | Output format                        | Normal output: "asr.utf-8"<br>Stream output: "asr.utf-8.stream"                                                              |
| input           | Input                                | KWS wake input: "kws.xxx" (input kws unit work_id)<br>Onboard microphone input: "sys.pcm"<br>UART stream input: "asr.wav.stream.base64" |
| rule1           | Timeout for unrecognized content wake | Unit: seconds                                                                                                               |
| rule2           | Maximum recognition interval         | Unit: seconds                                                                                                               |
| rule3           | Maximum recognition timeout          | Unit: seconds                                                                                                               |
| enoutput        | Enable UART output                   | Enable: true<br>Disable: false                                                                                                |

**Return Value:**

- String:
    - asr_work_id: ASR unit work_id

## ApiWhisper Class

The internal member `ApiWhisper whisper` of `M5ModuleLLM` is used to control the initialization and configuration of the Whisper unit.

### setup

**Function Prototype:**

```cpp
String setup(ApiWhisperSetupConfig_t config = ApiWhisperSetupConfig_t(), String request_id = "asr_setup",
```

**Function Description:**

- Initializes the Whisper unit and enables speech-to-text functionality.

**Input Parameters:**

ApiWhisperSetupConfig_t config:

- Whisper unit initialization configuration:
- String request_id:
    - Session ID, the default can be used.

```cpp
struct ApiAsrSetupConfig_t {
    String model           = "whisper-tiny";
    String response_format = "asr.utf-8";
    String input           = [ "sys.pcm", "kws.1000", "vad.1001" ];
    String language        = "en";
    bool enoutput          = true;
};
```

| Parameter       | Description                          | Input Values                                                                                                                |
| --------------- | ------------------------------------ | --------------------------------------------------------------------------------------------------------------------------- |
| model           | Conversion model                     | Model: "whisper-tiny"<br>                                                                                                    |
| response_format | Output format                        | Normal output: "asr.utf-8"<br>                                                                                               |
| input           | Input                                | KWS wake input: "kws.xxx" (input kws unit work_id)<br>Onboard microphone input: "sys.pcm"<br>UART stream input: "asr.wav.stream.base64" |
| language        | Language used for language recognition | Default: "en"<br>Optional: "zh", "ja"                                                                                         |
| enoutput        | Enable UART output                   | Enable: true<br>Disable: false                                                                                                |

**Return Value:**

- String:
    - whisper_work_id: Whisper unit work_id

## ApiLlm Class

The internal member `ApiLlm llm` of `M5ModuleLLM` is used to control the initialization and configuration of the LLM unit.

### setup

**Function prototype:**

```cpp
String setup(ApiLlmSetupConfig_t config = ApiLlmSetupConfig_t(), String request_id = "llm_setup");
```

**Function Description:**

- Initializes the LLM unit and supports configuring the input and output data format for the LLM unit.

**Parameters:**

- ApiLlmSetupConfig_t config:
    - LLM unit initialization configuration
- String request_id:
    - Session ID, the default value can be used.

```cpp
struct ApiLlmSetupConfig_t {
    String prompt;
    String model           = "qwen2.5-0.5B-prefill-20e";
    String response_format = "llm.utf-8.stream";
    String input           = ["llm.utf-8", "kws.1000"];
    bool enoutput          = true;
    int max_token_len      = 127;
};
```

| Parameter        | Description                                 | Input Values                                                                                                      |
| ---------------- | ------------------------------------------- | ----------------------------------------------------------------------------------------------------------------- |
| model            | Model used for conversion                   | Predefined model "qwen2.5-0.5B-prefill-20e"                                                                       |
| response_format  | Output format                               | Normal output: "llm.utf-8"<br>Streaming output: "llm.utf-8.stream"                                                 |
| input            | Input format                                | ASR input: "asr.xxx" (work_id of the ASR unit)<br>UART input: "llm.utf-8"<br>KWS wake-up interruption: "kws.xxx" (work_id of the KWS unit) |
| max_length       | Configures the maximum output token length (maximum returned inference text length) | Maximum value: 1023                                                                                                 |
| prompt           | Model initialization system prompt          | String                                                                                                            |
| enoutput         | Enable UART output                          | Enable: true<br>Disable: false                                                                                     |

**Return Value:**

- String:
    - `llm_work_id`: LLM unit work ID

### inference

**Function prototype:**

```cpp
int inference(String work_id, String input, String request_id = "llm_inference");
```

**Function Description:**

- Sends input data to start inference. The result will be placed in the `responseMsgList` container in `M5ModuleLLM.msg`.

**Parameters:**

- String work_id:
    - The LLM unit's work ID being called
- String input:
    - Input text
- String request_id:
    - Session ID, used to differentiate when multiple sessions exist.

**Return Value:**

- int:
    - `MODULE_LLM_OK` / Error Code

### inferenceAndWaitResult

**Function prototype:**

```cpp
int inferenceAndWaitResult(String work_id, String input, std::function<void(String&)> onResult, uint32_t timeout = 5000, String request_id = "llm_inference");
```

**Function Description:**

- Sends input data to start inference, blocks while waiting for the result, then calls the callback function.

**Parameters:**

- String work_id:
    - The LLM unit's work ID being called
- String input:
    - Input text
- void onResult(String&)
    - Callback function for inference result
- uint32_t timeout:
    - Timeout for waiting for inference result
- String request_id:
    - Session ID, used to differentiate when multiple sessions exist.

**Return Value:**

- int:
    - `MODULE_LLM_OK` / Error Code

## ApiVlm Class

The internal member `ApiVlm vlm` of `M5ModuleLLM` is used to control the initialization and configuration of the VLM unit.

### setup

**Function prototype:**

```cpp
String setup(ApiVlmSetupConfig_t config = ApiVlmSetupConfig_t(), String request_id = "vlm_setup");
```

**Function Description:**

- Initializes the VLM unit and supports configuring the input and output data format for the VLM unit.

**Parameters:**

- ApiVlmSetupConfig_t config:
    - VLM unit initialization configuration
- String request_id:
    - Session ID, the default value can be used.

```cpp
struct ApiVlmSetupConfig_t {
    String prompt;
    String model           = "internvl2.5-1B-ax630c";
    String response_format = "vlm.utf-8.stream";
    String input           = ["vlm.utf-8", "kws.1000"];
    bool enoutput          = true;
    int max_token_len      = 1023;
};
```

| Parameter        | Description                                 | Input Values                                                                                                      |
| ---------------- | ------------------------------------------- | ----------------------------------------------------------------------------------------------------------------- |
| model            | Model used for conversion                   | Predefined model "internvl2.5-1B-ax630c"                                                                          |
| response_format  | Output format                               | Normal output: "vlm.utf-8"<br>Streaming output: "vlm.utf-8.stream"                                                 |
| input            | Input format                                | ASR input: "asr.xxx" (work_id of the ASR unit)<br>UART input: "llm.utf-8"<br>KWS wake-up interruption: "kws.xxx" (work_id of the KWS unit) |
| max_length       | Configures the maximum output token length (maximum returned inference text length) | Maximum value: 1023                                                                                                 |
| prompt           | Model initialization system prompt          | String                                                                                                            |
| enoutput         | Enable UART output                          | Enable: true<br>Disable: false                                                                                     |

**Return Value:**

- String:
    - `vlm_work_id`: VLM unit work ID

### inference

**Function prototype:**

```cpp
int inference(String work_id, String input, String request_id = "vlm_inference");
```

**Function Description:**

- Sends input data to start inference. The result will be placed in the `responseMsgList` container in `M5ModuleLLM.msg`.

**Parameters:**

- String work_id:
    - The VLM unit's work ID being called
- String input:
    - Input text
- String request_id:
    - Session ID, used to differentiate when multiple sessions exist.

**Return Value:**

- int:
    - `MODULE_LLM_OK` / Error Code

### inferenceAndWaitResult

**Function prototype:**

```cpp
int inferenceAndWaitResult(String work_id, String input, std::function<void(String&)> onResult,
                           uint32_t timeout = 5000, String request_id = "vlm_inference");
```

**Function Description:**

- Sends input data to start inference, blocks while waiting for the result, then calls the callback function.

**Parameters:**

- String work_id:
    - The VLM unit's work ID being called
- String input:
    - Input text
- void onResult(String&)
    - Callback function for inference result
- uint32_t timeout:
    - Timeout for waiting for inference result
- String request_id:
    - Session ID, used to differentiate when multiple sessions exist.

**Return Value:**

- int:
    - `MODULE_LLM_OK` / Error Code

## ApiTts Class

The internal member `ApiTts tts` of `M5ModuleLLM` is used to control the initialization and configuration of the TTS unit.

### setup

**Function prototype:**

```cpp
String setup(ApiTtsSetupConfig_t config = ApiTtsSetupConfig_t(), String request_id = "tts_setup");
```

**Function description:**

- Initializes the TTS unit and enables the text-to-speech functionality.

**Parameters:**

- ApiTtsSetupConfig_t config:
    - LLM unit initialization configuration:
- String request_id:
    - Session ID, use the default if not needed.

```cpp
struct ApiTtsSetupConfig_t {
    String model           = "single_speaker_english_fast";
    String response_format = "sys.pcm";
    String input           = ["tts.utf-8.stream", "kws.1000"];
    bool enoutput          = false;
    bool enaudio           = true;
};
```

| Parameter | Description     | Input values                                                                                                                                          |
| --------- | -------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------- |
| model    | Conversion model | English model: "single_speaker_english_fast"<br>Chinese model: "single_speaker_fast"                                                                   |
| input    | Input          | LLM input: "llm.xxx" (input LLM unit's work_id)<br>UART input: "tts.utf-8"<br>UART stream input: "tts.utf-8.stream"<br>KWS wake-up interrupt: "kws.xxx" (input KWS unit's work_id) |
| enoutput | Enable UART output | Enable: true<br>Disable: false                                                                                                                       |
| enaudio  | Enable speaker playback | Enable: true<br>Disable: true                                                                                                                        |

**Return value:**

- String:
    - tts_work_id: TTS unit work_id

### inference

**Function prototype:**

```cpp
int inference(String work_id, String input, uint32_t timeout = 0, String request_id = "tts_inference");
```

**Function description:**

- Input data and start the inference conversion. After completion, the speaker will automatically play.

**Parameters:**

- String work_id:
    - Work ID of the TTS unit to be called.
- String input:
    - Input text.
- uint32_t timeout:
    - Timeout for waiting for inference.
- String request_id:
    - Session ID, used to distinguish between multiple sessions.

**Return value:**

- int:
    - MODULE_LLM_OK / Error Code

## ApiMelotts Class

The internal member `ApiMelotts melotts` of `M5ModuleLLM` is used to control the initialization and configuration of the Melotts unit.

### setup

**Function prototype:**

```cpp
String setup(ApiMelottsSetupConfig_t config = ApiMelottsSetupConfig_t(), String request_id = "melotts_setup", 
             String language = "en_US");
```

**Function description:**

- Initializes the Melotts unit and enables the text-to-speech functionality.

**Parameters:**

- ApiMelottsSetupConfig_t config:
    - Melotts unit initialization configuration:
- String request_id:
    - Session ID, use the default if not needed.

```cpp
struct ApiMelottsSetupConfig_t {
    String model              = "melotts_zh-cn";
    String response_format    = "sys.pcm";
    std::vector<String> input = {"tts.utf-8.stream"};
    bool enoutput             = false;
    bool enaudio              = true;
};
```

| Parameter | Description     | Input values                                                                                                                                          |
| --------- | -------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------- |
| model    | Conversion model | Chinese and English model: "melotts_zh-cn"<br>Chinese model: "single_speaker_fast"                                                                   |
| input    | Input          | LLM input: "llm.xxx" (input LLM unit's work_id)<br>UART input: "melotts.utf-8"<br>UART stream input: "melotts.utf-8.stream" |
| enoutput | Enable UART output | Enable: true<br>Disable: false                                                                                                                       |
| enaudio  | Enable speaker playback | Enable: true<br>Disable: true                                                                                                                        |

**Return value:**

- String:
    - melotts_work_id: Melotts unit work_id

### inference

**Function prototype:**

```cpp
int inference(String work_id, String input, uint32_t timeout = 0, String request_id = "tts_inference");
```

**Function description:**

- Input data and start the inference conversion. After completion, the speaker will automatically play.

**Parameters:**

- String work_id:
    - Work ID of the Melotts unit to be called.
- String input:
    - Input text.
- uint32_t timeout:
    - Timeout for waiting for inference.
- String request_id:
    - Session ID, used to distinguish between multiple sessions.

**Return value:**

- int:
    - MODULE_LLM_OK / Error Code

## ApiYolo Class

The internal member `ApiYolo yolo` of `M5ModuleLLM` is used to control the initialization and configuration of the Yolo unit.

### setup

**Function prototype:**

```cpp
String setup(ApiYoloSetupConfig_t config = ApiYoloSetupConfig_t(), String request_id = "yolo_setup");
```

**Function description:**

- Initializes the Yolo unit and enables image detection functionality.

**Parameters:**

- ApiYoloSetupConfig_t config:
    - Yolo unit initialization configuration:
- String request_id:
    - Session ID, use the default if not needed.

```cpp
struct ApiYoloSetupConfig_t {
    String model              = "yolo11n";
    String response_format    = "yolo.box.stream";
    std::vector<String> input = {"yolo.jpeg.base64"};
    bool enoutput             = true;
};
```

| Parameter | Description     | Input values                                                                                                                                          |
| --------- | -------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------- |
| model    | Conversion model | Detection model: "yolo11n"<br>Pose model: "yolo11n-pose"<br>Hand pose model: "yolo11n-hand-pose"                                                      |
| response_format | Output format | Detection output: "yolo.box.stream"<br>Pose output: "yolo.pose.stream"                                                                 |
| input    | Input          | UVC input: "camera.xxx" (input camera unit's work_id)<br>UART stream input: "yolo.jpeg.base64.stream" |
| enoutput | Enable UART output | Enable: true<br>Disable: false                                                                                                                       |

**Return value:**

- String:
    - yolo_work_id: Yolo unit work_id

## ModuleMsg Class

The internal member `ModuleMsg msg` of `M5ModuleLLM` provides a container `responseMsgList` used to cache various information returned from the LLM Module. Refer to the following example, where the main loop iterates to retrieve the results.

```cpp
void loop()
{
    module_llm.update();

    // Handle response msg
    for (auto& msg : module_llm.msg.responseMsgList) {
        // KWS msg
        if (msg.work_id == kws_work_id) {
            Serial.printf(">> Keyword detected\n");
        }

        // ASR msg
        if (msg.work_id == asr_work_id) {
            if (msg.object == "asr.utf-8.stream") {
                // Parse and get asr result
                JsonDocument doc;
                deserializeJson(doc, msg.raw_msg);
                String asr_result = doc["data"]["delta"].as<String>();
                Serial.printf(">> %s\n", asr_result.c_str());
            }
        }
    }
    module_llm.msg.responseMsgList.clear();
}
```

## VoiceAssistant Class

`M5ModuleLLM_VoiceAssistant` is used to quickly create an LLM voice assistant instance, achieving a fast implementation of KWS (keyword spotting) -> ASR (speech-to-text) -> LLM (large model inference) -> TTS (text-to-speech).

- During initialization, simply pass the `M5ModuleLLM` instance to the constructor, and register the corresponding event callback functions to complete the voice assistant setup.

```cpp
/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#include <Arduino.h>
#include <M5Unified.h>
#include <M5ModuleLLM.h>

M5ModuleLLM module_llm;
M5ModuleLLM_VoiceAssistant voice_assistant(&module_llm);

/* On ASR data callback */
void on_asr_data_input(String data, bool isFinish, int index)
{
    M5.Display.setTextColor(TFT_GREEN, TFT_BLACK);
    M5.Display.printf(">> %s\n", data.c_str());

    /* If ASR data is finish */
    if (isFinish) {
        M5.Display.setTextColor(TFT_YELLOW, TFT_BLACK);
        M5.Display.print(">> ");
    }
};

/* On LLM data callback */
void on_llm_data_input(String data, bool isFinish, int index)
{
    M5.Display.print(data);

    /* If LLM data is finish */
    if (isFinish) {
        M5.Display.print("\n");
    }
};

void setup()
{
    M5.begin();
    M5.Display.setTextSize(2);
    M5.Display.setTextScroll(true);

    /* Init module serial port */
    Serial2.begin(115200, SERIAL_8N1, 16, 17);  // Basic
    // Serial2.begin(115200, SERIAL_8N1, 13, 14);  // Core2
    // Serial2.begin(115200, SERIAL_8N1, 18, 17);  // CoreS3

    /* Init module */
    module_llm.begin(&Serial2);

    /* Make sure module is connected */
    M5.Display.printf(">> Check ModuleLLM connection..\n");
    while (1) {
        if (module_llm.checkConnection()) {
            break;
        }
    }

    /* Begin voice assistant preset */
    M5.Display.printf(">> Begin voice assistant..\n");
    int ret = voice_assistant.begin("HELLO");
    if (ret != MODULE_LLM_OK) {
        while (1) {
            M5.Display.setTextColor(TFT_RED);
            M5.Display.printf(">> Begin voice assistant failed\n");
        }
    }

    /* Register on ASR data callback function */
    voice_assistant.onAsrDataInput(on_asr_data_input);

    /* Register on LLM data callback function */
    voice_assistant.onLlmDataInput(on_llm_data_input);

    M5.Display.printf(">> Voice assistant ready\n");
}

void loop()
{
    /* Keep voice assistant preset update */
    voice_assistant.update();
}
```

## Error Code

```cpp
enum ModuleLLMErrorCode_t {
    MODULE_LLM_OK                              = 0,
    MODULE_LLM_RESET_WARN                      = -1,
    MODULE_LLM_JSON_FORMAT_ERROR               = -2,
    MODULE_LLM_ACTION_MATCH_FAILED             = -3,
    MODULE_LLM_INFERENCE_DATA_PUSH_FAILED      = -4,
    MODULE_LLM_MODEL_LOADING_FAILED            = -5,
    MODULE_LLM_UNIT_NOT_EXIST                  = -6,
    MODULE_LLM_UNKNOWN_OPERATION               = -7,
    MODULE_LLM_UNIT_RESOURCE_ALLOCATION_FAILED = -8,
    MODULE_LLM_UNIT_CALL_FAILED                = -9,
    MODULE_LLM_MODEL_INIT_FAILED               = -10,
    MODULE_LLM_MODEL_RUN_FAILED                = -11,
    MODULE_LLM_MODULE_NOT_INITIALISED          = -12,
    MODULE_LLM_MODULE_ALREADY_WORKING          = -13,
    MODULE_LLM_MODULE_NOT_WORKING              = -14,
    MODULE_LLM_NO_UPDATEABLE_MODULES           = -15,
    MODULE_LLM_NO_MODULES_AVAILABLE_FOR_UPDATE = -16,
    MODULE_LLM_FILE_OPEN_FAILED                = -17,
    MODULE_LLM_WAIT_RESPONSE_TIMEOUT           = -97,
    MODULE_LLM_RESPONSE_PARSE_FAILED           = -98,
    MODULE_LLM_ERROR_NONE                      = -99,
};
```