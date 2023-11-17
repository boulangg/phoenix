/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <cstdint>

namespace kernel::core::rtc {

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

    const DateTime& getCurrentDateTime() const
    {
        return _currentDateTime;
    }

private:
    static constexpr std::uint32_t CURRENT_YEAR = 2023;

    RTCDevice() {}

    int IRQHandler(std::uint8_t, void*);
    void readDateAndTime();

    DateTime _currentDateTime;
};

}
