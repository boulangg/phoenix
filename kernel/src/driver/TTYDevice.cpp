/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "TTYDevice.hpp"

struct keybind normal_keybind
{
    '\0', 0x1b, '&', 'e', '"', '\'', '(', '-', 'e', '_', 'c', 'a', ')', '=', '\b', 0x09, 'a', 'z', 'e', 'r', 't', 'y',
        'u', 'i', 'o', 'p', '^', '$', '\n', 0x00, 'q', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm', 'u', 0x60, 0x00,
        '\\', 'w', 'x', 'c', 'v', 'b', 'n', ',', ';', ':', '!', 0x00, '*', 0x00, ' ',
};
