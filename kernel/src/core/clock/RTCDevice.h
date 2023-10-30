/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT
 * license
 * The license is available in the LICENSE file or at
 * https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include <cstdint>

#include "core/InterruptDispatcher.h"

namespace kernel::core::clock {

struct DateTime
{
    std::uint8_t second;
    std::uint8_t minute;
    std::uint8_t hour;
    std::uint8_t day;
    std::uint8_t month;
    std::uint32_t year;
};

class RTCDevice
{
public:
    static RTCDevice* initRTC();

private:
    RTCDevice() {}

    int IRQHandler(std::uint8_t, void*);
    void readDateAndTime();

    InterruptHandler* _handler;
};

}
