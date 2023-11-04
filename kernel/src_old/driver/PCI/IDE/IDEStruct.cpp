/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include <driver/PCI/IDE/IDEStruct.hpp>

template <int s, int t>
struct check_size
{
    static_assert(s == t, "wrong size");
};

#define offsetof(st, m) __builtin_offsetof(st, m)

check_size<sizeof(IDEIdentifyData), 512> test1;
check_size<offsetof(IDEIdentifyData, logicalSectorSize), 234> test2;

static_assert(sizeof(IDEIdentifyData) == 512, "Size of IDEIdentifyData struct is not correct");

// static_assert (offsetof(logicalSectorSize) == 117, "incorrect offset for logicalSectorSize");
