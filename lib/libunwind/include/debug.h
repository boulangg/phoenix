/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include "stdio.h"

#define VA_ARGS(...) , ##__VA_ARGS__
#define DEBUG_LOG(fmt, ...)                                                                                            \
    do {                                                                                                               \
        fprintf(stderr, "## %s(): " fmt, __func__, ##__VA_ARGS__);                                                     \
    } while (0)
