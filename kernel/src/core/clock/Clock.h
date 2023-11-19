/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <ctime>

#include "asm/cpu.h"

namespace kernel::core::clock {

struct BaseTime
{
    time_t time;
    std::uint64_t tsc;
};

static constexpr std::int64_t NSEC_PER_SEC = 1000000000ull;

class Clock
{
public:
    void setCpuSpeed(std::uint64_t speed)
    {
        _speed = speed;
    }

    void setCurrentTime(BaseTime baseTime)
    {
        _baseTime = baseTime;
    }

    bool isClockInitialized()
    {
        return _baseTime.time != 0 && _speed != 0;
    }

    timespec getTime()
    {
        BaseTime base = _baseTime;
        std::int64_t tsc = std::int64_t(rdtsc() - base.tsc);
        std::int64_t nsec = tsc * NSEC_PER_SEC / _speed;
        timespec res = {.tv_sec = base.time + nsec / NSEC_PER_SEC, .tv_nsec = std::int32_t(nsec % NSEC_PER_SEC)};
        return res;
    }

private:
    BaseTime _baseTime;
    std::uint64_t _speed;
};
}
