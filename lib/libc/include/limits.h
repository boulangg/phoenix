/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include "bits/wordsize.h"

// General
#define __STDC_VERSION_LIMITS_H__ 202311L

// Characteristics of integer types
#define BOOL_WIDTH      (sizeof(bool) * 8)
#define CHAR_BIT        (sizeof(char) * 8)
#define USHRT_WIDTH     (sizeof(unsigned short) * 8)
#define UINT_WIDTH      (sizeof(unsigned int) * 8)
#define ULONG_WIDTH     (sizeof(unsigned long) * 8)
#define ULLONG_WIDTH    (sizeof(unsigned long long) * 8)
#define BITINT_MAXWIDTH ULLONG_WIDTH

#define MB_LEN_MAX 16

#define BOOL_MAX 1

#define SCHAR_MIN (-128)
#define SCHAR_MAX (+127)
#define UCHAR_MAX 255

#ifdef __CHAR_UNSIGNED__
#define CHAR_MIN 0
#define CHAR_MAX UCHAR_MAX
#else
#define CHAR_MIN SCHAR_MIN
#define CHAR_MAX SCHAR_MAX
#endif // __CHAR_UNSIGNED__

#define SHRT_MIN  (-32768)
#define SHRT_MAX  (+32767)
#define USHRT_MAX 65535

#define INT_MIN  (-2147483648)
#define INT_MAX  (+2147483647)
#define UINT_MAX 4294967295U

#define LLONG_MIN  (-9223372036854775808LL)
#define LLONG_MAX  (+9223372036854775807LL)
#define ULLONG_MAX 18446744073709551615ULL

#if __WORDSIZE == 64
#define LONG_MIN  LLONG_MIN
#define LONG_MAX  LLONG_MAX
#define ULONG_MAX ULLONG_MAX
#else
#define LONG_MIN  INT_MIN
#define LONG_MAX  INT_MAX
#define ULONG_MAX UINT_MAX
#endif // __WORDSIZE == 64
