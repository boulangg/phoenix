/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include "asm/cpu.h"

#include "InputDevice.h"
#include "InputManager.h"

namespace kernel::dev::input {

class KeyboardDevice : public InputDevice
{
public:
    static void initKeyboard();

    KeyboardDevice() : InputDevice("Keyboard") {}

    virtual ~KeyboardDevice() {}

    int IRQHandler(std::uint8_t, void*)
    {
        processScancode();
        return 0;
    }

    virtual int handleEvent(unsigned int, unsigned int, int) override
    {
        return 0;
    }

protected:
    void processScancode();

    char _previousScancode;
};

}
