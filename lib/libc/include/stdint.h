/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#define __STDC_VERSION_STDINT_H__ 202311L

#include <bits/wordsize.h>

// Integer types
/// Exact Minimun-width integer types
typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

#if __WORDSIZE == 64
typedef signed long int64_t;
typedef unsigned long uint64_t;
#else
typedef signed long long int64_t;
typedef unsigned long long uint64_t;
#endif

typedef int64_t intmax_t;
typedef uint64_t uintmax_t;

/// Minimun-width integer types
typedef int8_t int_least8_t;
typedef int16_t int_least16_t;
typedef int32_t int_least32_t;
typedef int64_t int_least64_t;

typedef uint8_t uint_least8_t;
typedef uint16_t uint_least16_t;
typedef uint32_t uint_least32_t;
typedef uint64_t uint_least64_t;

/// Fastest minimum-width integer types
typedef int8_t int_fast8_t;
typedef int16_t int_fast16_t;
typedef int32_t int_fast32_t;
typedef int64_t int_fast64_t;

typedef uint8_t uint_fast8_t;
typedef uint16_t uint_fast16_t;
typedef uint32_t uint_fast32_t;
typedef uint64_t uint_fast64_t;

/// Integer types capable of holding object pointers
#if __WORDSIZE == 64
typedef int64_t intptr_t;
typedef uint64_t uintptr_t;
#else
typedef int32_t intptr_t;
typedef uint32_t uintptr_t;
#endif

/// Greatest-width integer types
typedef int64_t intmax_t;
typedef uint64_t uintmax_t;

// Widths of specified-width integer types
/// Width of exact-width integer types
#define INT8_WIDTH  (sizeof(int8_t) * 8)
#define INT16_WIDTH (sizeof(int16_t) * 8)
#define INT32_WIDTH (sizeof(int32_t) * 8)
#define INT64_WIDTH (sizeof(int64_t) * 8)

#define UINT8_WIDTH  (sizeof(uint8_t) * 8)
#define UINT16_WIDTH (sizeof(uint16_t) * 8)
#define UINT32_WIDTH (sizeof(uint32_t) * 8)
#define UINT64_WIDTH (sizeof(uint64_t) * 8)

/// Width of minimum-width integer types
#define INT_LEAST8_WIDTH  (sizeof(int_least8_t) * 8)
#define INT_LEAST16_WIDTH (sizeof(int_least16_t) * 8)
#define INT_LEAST32_WIDTH (sizeof(int_least32_t) * 8)
#define INT_LEAST64_WIDTH (sizeof(int_least64_t) * 8)

#define UINT_LEAST8_WIDTH  (sizeof(uint_least8_t) * 8)
#define UINT_LEAST16_WIDTH (sizeof(uint_least16_t) * 8)
#define UINT_LEAST32_WIDTH (sizeof(uint_least32_t) * 8)
#define UINT_LEAST64_WIDTH (sizeof(uint_least64_t) * 8)

/// Width of fastest minimum-width integer types
#define INT_FAST8_WIDTH  (sizeof(int_fast8_t) * 8)
#define INT_FAST16_WIDTH (sizeof(int_fast16_t) * 8)
#define INT_FAST32_WIDTH (sizeof(int_fast32_t) * 8)
#define INT_FAST64_WIDTH (sizeof(int_fast64_t) * 8)

#define UINT_FAST8_WIDTH  (sizeof(uint_fast8_t) * 8)
#define UINT_FAST16_WIDTH (sizeof(uint_fast16_t) * 8)
#define UINT_FAST32_WIDTH (sizeof(uint_fast32_t) * 8)
#define UINT_FAST64_WIDTH (sizeof(uint_fast64_t) * 8)

/// Width of integer types capable of holding object pointers
#define INTPTR_WIDTH  (sizeof(intptr_t) * 8)
#define UINTPTR_WIDTH (sizeof(uintptr_t) * 8)

/// Width of greatest-width integer types
#define INTMAX_WIDTH  (sizeof(intmax_t) * 8)
#define UINTMAX_WIDTH (sizeof(uintmax_t) * 8)

/// Width of other integer types
#define PTRDIFF_WIDTH (sizeof(signed long) * 8)
#define SIZE_WIDTH    (sizeof(unsigned long) * 8)
#define WCHAR_WIDTH   (sizeof(wchar_t) * 8)
#define WINT_WIDTH    (sizeof(wchar_t) * 8)

// Macros for constants of integer type
/// Macros for minimum-width constants
#define INT8_C(VALUE)  (VALUE)
#define INT16_C(VALUE) (VALUE)
#define INT32_C(VALUE) (VALUE)
#define INT64_C(VALUE) (VALUE##LL)

#define UINT8_C(VALUE)  (VALUE##U)
#define UINT16_C(VALUE) (VALUE##U)
#define UINT32_C(VALUE) (VALUE##U)
#define UINT64_C(VALUE) (VALUE##ULL)

/// Macros for greatest-width constants
#define INT_MAX_C(VALUE)  INT64_C(VALUE)
#define UINT_MAX_C(VALUE) UINT64_C(VALUE)

// Maximal and minimal values of integer types
/// Limits of exact-width integer types
#define INT8_MIN  SCHAR_MIN
#define INT16_MIN SHRT_MIN
#define INT32_MIN INT_MIN
#define INT64_MIN LLONG_MIN

#define INT8_MAX  SCHAR_MAX
#define INT16_MAX SHRT_MAX
#define INT32_MAX INT_MAX
#define INT64_MAX LLONG_MAX

#define UINT8_MAX  UCHAR_MAX
#define UINT16_MAX USHRT_MAX
#define UINT32_MAX UINT_MAX
#define UINT64_MAX ULLONG_MAX

/// Limits of minimum-width integer types
#define INT_LEAST8_MIN  SCHAR_MIN
#define INT_LEAST16_MIN SHRT_MIN
#define INT_LEAST32_MIN INT_MIN
#define INT_LEAST64_MIN LLONG_MIN

#define INT_LEAST8_MAX  SCHAR_MAX
#define INT_LEAST16_MAX SHRT_MAX
#define INT_LEAST32_MAX INT_MAX
#define INT_LEAST64_MAX LLONG_MAX

#define UINT_LEAST8_MAX  UCHAR_MAX
#define UINT_LEAST16_MAX USHRT_MAX
#define UINT_LEAST32_MAX UINT_MAX
#define UINT_LEAST64_MAX ULLONG_MAX

/// Limits of fastest minimum-width integer types
#define INT_FAST8_MIN  SCHAR_MIN
#define INT_FAST16_MIN SHRT_MIN
#define INT_FAST32_MIN INT_MIN
#define INT_FAST64_MIN LLONG_MIN

#define INT_FAST8_MAX  SCHAR_MAX
#define INT_FAST16_MAX SHRT_MAX
#define INT_FAST32_MAX INT_MAX
#define INT_FAST64_MAX LLONG_MAX

#define UINT_FAST8_MAX  UCHAR_MAX
#define UINT_FAST16_MAX USHRT_MAX
#define UINT_FAST32_MAX UINT_MAX
#define UINT_FAST64_MAX ULLONG_MAX

/// Limits of integer types capable of holding object pointers
#if __WORDSIZE == 64
#define INTPTR_MIN  LLONG_MIN
#define INTPTR_MAX  LLONG_MAX
#define UINTPTR_MAX ULLONG_MAX
#else
#define INTPTR_MIN  INT_MIN
#define INTPTR_MAX  INT_MAX
#define UINTPTR_MAX UINT_MAX
#endif // __WORDSIZE == 64

/// limits of greatest-width integer types
#define INTMAX_MIN  LLONG_MIN
#define INTMAX_MAX  LLONG_MAX
#define UINTMAX_MAX ULLONG_MAX

/// Limits of other integer types

#define PTRDIFF_MIN LONG_MIN
#define PTRDIFF_MAX LONG_MAX

// #define SIG_ATOMIC_MIN
// #define SIG_ATOMIC_MAX

#define SIZE_MAX LONG_MAX

// #define WCHAR_MIN	INT_MIN
// #define WCHAR_MAX	INT_MAX

// #define WINT_MIN	INT_MIN
// #define WINT_MAX	INT_MAX
