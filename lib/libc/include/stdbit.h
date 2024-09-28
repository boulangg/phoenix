/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

// General
#define __STDC_VERSION_STDBIT_H__ 202311L

// Endian
#define __STDC_ENDIAN_LITTLE__ __ORDER_LITTLE_ENDIAN__
#define __STDC_ENDIAN_BIG__    __ORDER_BIG_ENDIAN__
#define __STDC_ENDIAN_NATIVE__ __BYTE_ORDER__

// Count Leading Zeros
unsigned int stdc_leading_zeros_uc(unsigned char value);
unsigned int stdc_leading_zeros_us(unsigned short value);
unsigned int stdc_leading_zeros_ui(unsigned int value);
unsigned int stdc_leading_zeros_ul(unsigned long int value);
unsigned int stdc_leading_zeros_ull(unsigned long long int value);
#define stdc_leading_zeros(value)                                                                                      \
    (stdc_leading_zeros_ull((unsigned long long)(value)) - (sizeof(0ULL) - sizeof(value)) * 8)

// Count Leading Ones
unsigned int stdc_leading_ones_uc(unsigned char value);
unsigned int stdc_leading_ones_us(unsigned short value);
unsigned int stdc_leading_ones_ui(unsigned int value);
unsigned int stdc_leading_ones_ul(unsigned long int value);
unsigned int stdc_leading_ones_ull(unsigned long long int value);
#define stdc_leading_ones(value)                                                                                       \
    (stdc_leading_zeros_ull(~value & (-1ull >> ((sizeof(0ULL) - sizeof(value)) * 8))) - sizeof(0ULL) * 8 +             \
     sizeof(value) * 8)

// Count Trailing Zeros
unsigned int stdc_trailing_zeros_uc(unsigned char value);
unsigned int stdc_trailing_zeros_us(unsigned short value);
unsigned int stdc_trailing_zeros_ui(unsigned int value);
unsigned int stdc_trailing_zeros_ul(unsigned long int value);
unsigned int stdc_trailing_zeros_ull(unsigned long long int value);
#define __stdc_min(a, b) ((a <= b) ? a : b)
#define stdc_trailing_zeros(value) __stdc_min((sizeof(value) * 8), stdc_trailing_zeros_ull((unsigned long long)(value)))

// Count Trailing Ones
unsigned int stdc_trailing_ones_uc(unsigned char value);
unsigned int stdc_trailing_ones_us(unsigned short value);
unsigned int stdc_trailing_ones_ui(unsigned int value);
unsigned int stdc_trailing_ones_ul(unsigned long int value);
unsigned int stdc_trailing_ones_ull(unsigned long long int value);
#define stdc_trailing_ones(value) stdc_trailing_zeros(~value)

// First Leading Zero
unsigned int stdc_first_leading_zero_uc(unsigned char value);
unsigned int stdc_first_leading_zero_us(unsigned short value);
unsigned int stdc_first_leading_zero_ui(unsigned int value);
unsigned int stdc_first_leading_zero_ul(unsigned long int value);
unsigned int stdc_first_leading_zero_ull(unsigned long long int value);
#define stdc_first_leading_zero(value) (sizeof(value) * 8 - stdc_leading_ones(value))

// First Leading One
unsigned int stdc_first_leading_one_uc(unsigned char value);
unsigned int stdc_first_leading_one_us(unsigned short value);
unsigned int stdc_first_leading_one_ui(unsigned int value);
unsigned int stdc_first_leading_one_ul(unsigned long int value);
unsigned int stdc_first_leading_one_ull(unsigned long long int value);
#define stdc_first_leading_one(value) (sizeof(value) * 8 - stdc_leading_zeros(value))

// First Trailing Zero
unsigned int stdc_first_trailing_zero_uc(unsigned char value);
unsigned int stdc_first_trailing_zero_us(unsigned short value);
unsigned int stdc_first_trailing_zero_ui(unsigned int value);
unsigned int stdc_first_trailing_zero_ul(unsigned long int value);
unsigned int stdc_first_trailing_zero_ull(unsigned long long int value);
#define stdc_first_trailing_zero(value) (sizeof(value) * 8 - stdc_trailing_ones(value))

// First Trailing One
unsigned int stdc_first_trailing_one_uc(unsigned char value);
unsigned int stdc_first_trailing_one_us(unsigned short value);
unsigned int stdc_first_trailing_one_ui(unsigned int value);
unsigned int stdc_first_trailing_one_ul(unsigned long int value);
unsigned int stdc_first_trailing_one_ull(unsigned long long int value);
#define stdc_first_trailing_one(value) (sizeof(value) * 8 - stdc_trailing_zeros(value))

// Count Zeros
unsigned int stdc_count_zeros_uc(unsigned char value);
unsigned int stdc_count_zeros_us(unsigned short value);
unsigned int stdc_count_zeros_ui(unsigned int value);
unsigned int stdc_count_zeros_ul(unsigned long int value);
unsigned int stdc_count_zeros_ull(unsigned long long int value);
#define stdc_count_zeros(value)                                                                                        \
    (stdc_count_zeros_ull((unsigned long long)(value)) - (sizeof(0ULL) - sizeof(value)) * 8)

// Count Ones
unsigned int stdc_count_ones_uc(unsigned char value);
unsigned int stdc_count_ones_us(unsigned short value);
unsigned int stdc_count_ones_ui(unsigned int value);
unsigned int stdc_count_ones_ul(unsigned long int value);
unsigned int stdc_count_ones_ull(unsigned long long int value);
#define stdc_count_ones(value) (stdc_count_ones_ull((unsigned long long)(value)))

// Single-bit Check
bool stdc_has_single_bit_uc(unsigned char value);
bool stdc_has_single_bit_us(unsigned short value);
bool stdc_has_single_bit_ui(unsigned int value);
bool stdc_has_single_bit_ul(unsigned long int value);
bool stdc_has_single_bit_ull(unsigned long long int value);
#define stdc_has_single_bit(value) (stdc_count_ones(value) == 1)

// Bit Width
unsigned int stdc_bit_width_uc(unsigned char value);
unsigned int stdc_bit_width_us(unsigned short value);
unsigned int stdc_bit_width_ui(unsigned int value);
unsigned int stdc_bit_width_ul(unsigned long int value);
unsigned int stdc_bit_width_ull(unsigned long long int value);
#define stdc_bit_width(value) (stdc_bit_width_ull((unsigned long long)(value)))

// Bit Floor
unsigned char stdc_bit_floor_uc(unsigned char value);
unsigned short stdc_bit_floor_us(unsigned short value);
unsigned int stdc_bit_floor_ui(unsigned int value);
unsigned long int stdc_bit_floor_ul(unsigned long int value);
unsigned long long int stdc_bit_floor_ull(unsigned long long int value);
#define stdc_bit_floor(value) (stdc_bit_floor_ull((unsigned long long)(value)))

// Bit Ceiling
unsigned char stdc_bit_ceil_uc(unsigned char value);
unsigned short stdc_bit_ceil_us(unsigned short value);
unsigned int stdc_bit_ceil_ui(unsigned int value);
unsigned long int stdc_bit_ceil_ul(unsigned long int value);
unsigned long long int stdc_bit_ceil_ull(unsigned long long int value);
#define stdc_bit_ceil(value)                                                                                           \
    (((unsigned long long)(value) <= 1) ? 1 : ((stdc_leading_zeros(value) == 0) ? 0 : (1 << stdc_bit_width(value - 1))))

#ifdef __cplusplus
}
#endif
