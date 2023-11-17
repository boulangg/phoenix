/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <cstdint>
#include <sys/times.h>

namespace kernel::dev::input {

struct InputEvent
{
    struct timeval time;
    std::uint16_t type;
    std::uint16_t code;
    std::int32_t value;
};

}
