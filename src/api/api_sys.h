/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once
#include "../utils/msg.h"
#include <Arduino.h>

namespace m5_module_llm {

class ApiSys {
public:
    void init(ModuleMsg* moduleMsg);

    int ping();
    int reset(bool waitResetFinish = true);
    int reboot();

private:
    ModuleMsg* _module_msg = nullptr;
};

}  // namespace m5_module_llm
