/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

namespace kernel::console::keymap {

struct keymap_entry
{
    char base;
    char maj;
    char ctrl_alt;
};

struct keymap
{
    keymap_entry entries[256];
};

}
