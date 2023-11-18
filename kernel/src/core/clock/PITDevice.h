/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include "core/InterruptDispatcher.h"
#include <cstdint>

namespace kernel::core::clock {

enum PITChannel
{
    CHANNEL_0 = 0b00, // interrupt on terminal count
    CHANNEL_1 = 0b01, // hardware re-triggerable one-shot
    CHANNEL_2 = 0b10, // rate generator
};

enum PITMode
{
    MODE_0 = 0b000, // interrupt on terminal count
    MODE_1 = 0b001, // hardware re-triggerable one-shot
    MODE_2 = 0b010, // rate generator
    MODE_3 = 0b011, // square wave generator
    MODE_4 = 0b100, // software triggered strobe
    MODE_5 = 0b101, // hardware triggered strobe
};

class PITDevice
{
public:
    static PITDevice* initPIT();

    void switchHandler();

private:
    int measureCpuSpeed(std::uint8_t, void*);
    int schedule(std::uint8_t, void*);
    void changeConfig(PITChannel channel, PITMode mode, std::uint16_t divider);

    InterruptHandler* _handler;
};

}
