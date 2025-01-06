# LLM Module Arduino Quick Start

## Overview

The `LLM Module` can be used with various M5 controllers. This tutorial demonstrates how to control the LLM Module using the `M5Core` series in the `Arduino IDE` with the LLM Module driver library.

<img src="https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/docs/static/assets/img/guide/llm/llm/llm_module_device_01.jpg" width="70%" />

## Environment Setup

- 1.Arduino IDE Installation: Refer to the [Arduino IDE Installation Guide](/en/arduino/arduino_ide) to complete the IDE installation.

- 2.Board Manager Installation: Refer to the [Basic Environment Setup Guide](/en/arduino/arduino_board) to complete the M5Stack board manager installation and select the `M5Core` development board.

<img src="https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/docs/static/assets/img/arduino/m5core/quickstart_arduino_core_selectboard.png" width="70%" />

- 3.Library Installation: Refer to the [Library Management Guide](/en/arduino/arduino_library) to install the `LLM Module` driver library. (Follow prompts to install the dependency library `M5Unified`)

<img src="https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/docs/static/assets/img/guide/llm/llm/llm_arduino_lib_01.jpg" width="70%" />
<img src="https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/docs/static/assets/img/guide/llm/llm/llm_arduino_lib_02.jpg" width="70%" />

## Program Compilation & Upload

Open the example program "kws_asr" in the driver library, click the upload button, and the program will automatically compile and upload.The wake-up word used in the example program is "HELLO". After waiting for the device to be initialized, it will be woken up using the keyword.

<img src="https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/docs/static/assets/img/guide/llm/llm/llm_arduino_example_01.jpg" width="70%" />
<img src="https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/docs/static/assets/img/guide/llm/llm/llm_arduino_example_02.jpg" width="70%" />
<img src="https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/docs/static/assets/img/guide/llm/llm/llm_arduino_example_03.jpg" width="70%" />
<img src="https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/docs/static/assets/img/guide/llm/llm/llm_arduino_example_04.jpg" width="70%" />

- Examples:
  - `kws_asr`: Uses KWS to wake up and triggers ASR for speech-to-text conversion. (KWS+ASR)
  - `text_assistant`: Inputs text into the LLM model, performs inference, and outputs the result in text form. (LLM)
  - `tts`: Uses the TTS unit to convert text to speech for playback. (TTS)
  - `voice_assistant`: Uses KWS to wake up, triggers ASR for speech-to-text conversion, inputs the converted text into the LLM for inference, and outputs the inference result through TTS as speech. (KWS+ASR+LLM+TTS)

## Related Links

- [LLM Module Arduino Lib](https://github.com/m5stack/M5Module-LLM)
- [LLM Module Arduino Lib API](/en/guide/llm/llm/arduino_api)

