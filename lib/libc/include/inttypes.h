/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include <bits/wordsize.h>

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

// General
//#define __STDC_VERSION_INTTYPES_H__ 202311L

struct imaxdiv_t
{
    intmax_t quot;
    intmax_t rem;
};

intmax_t imaxabs(intmax_t j);

#ifdef __cplusplus
}
#endif

