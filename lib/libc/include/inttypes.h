/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef __INTTYPES_H__
#define __INTTYPES_H__

#include <bits/wordsize.h>

typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

#if __WORDSIZE == 64
typedef signed long int64_t;
typedef unsigned long uint64_t;
typedef int64_t intptr_t;
typedef uint64_t uintptr_t;
#else
typedef signed long long int64_t;
typedef unsigned long long uint64_t;
typedef int32_t intptr_t;
typedef uint32_t uintptr_t;
#endif

#endif // __INTTYPES_H__
