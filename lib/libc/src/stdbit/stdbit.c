/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #include <stdbit.h>

unsigned int stdc_leading_zeros_uc(unsigned char value)
{
    if (value == 0) {
        return sizeof(unsigned char) * 8;
    }
    return __builtin_clz(value) - sizeof(unsigned int) * 8 + sizeof(unsigned char) * 8;
}

unsigned int stdc_leading_zeros_us(unsigned short value)
{
    if (value == 0) {
        return sizeof(unsigned short) * 8;
    }
    return __builtin_clz(value) - sizeof(unsigned int) * 8 + sizeof(unsigned short) * 8;
}

unsigned int stdc_leading_zeros_ui(unsigned int value)
{
    if (value == 0) {
        return sizeof(unsigned int) * 8;
    }
    return __builtin_clz(value);
}

unsigned int stdc_leading_zeros_ul(unsigned long int value)
{
    if (value == 0) {
        return sizeof(unsigned long int) * 8;
    }
    return __builtin_clzl(value);
}

unsigned int stdc_leading_zeros_ull(unsigned long long int value)
{
    if (value == 0) {
        return sizeof(unsigned long long int) * 8;
    }
    return __builtin_clzll(value);
}

unsigned int stdc_leading_ones_uc(unsigned char value)
{
    return stdc_leading_zeros_uc(~value);
}

unsigned int stdc_leading_ones_us(unsigned short value)
{
    return stdc_leading_zeros_us(~value);
}

unsigned int stdc_leading_ones_ui(unsigned int value)
{
    return stdc_leading_zeros_ui(~value);
}

unsigned int stdc_leading_ones_ul(unsigned long int value)
{
    return stdc_leading_zeros_ul(~value);
}

unsigned int stdc_leading_ones_ull(unsigned long long int value)
{
    return stdc_leading_zeros_ull(~value);
}

unsigned int stdc_trailing_zeros_uc(unsigned char value)
{
    if (value == 0) {
        return sizeof(unsigned char) * 8;
    }
    unsigned int ctz = __builtin_ctz(value);
    return (ctz >= sizeof(unsigned char) * 8) ? sizeof(unsigned char) * 8 : ctz;
}

unsigned int stdc_trailing_zeros_us(unsigned short value)
{
    if (value == 0) {
        return sizeof(unsigned short) * 8;
    }
    unsigned int ctz = __builtin_ctz(value);
    return (ctz >= sizeof(unsigned short) * 8) ? sizeof(unsigned short) * 8 : ctz;
}

unsigned int stdc_trailing_zeros_ui(unsigned int value)
{
    if (value == 0) {
        return sizeof(unsigned int) * 8;
    }
    return __builtin_ctz(value);
}

unsigned int stdc_trailing_zeros_ul(unsigned long int value)
{
    if (value == 0) {
        return sizeof(unsigned long int) * 8;
    }
    return __builtin_ctzl(value);
}

unsigned int stdc_trailing_zeros_ull(unsigned long long int value)
{
    if (value == 0) {
        return sizeof(unsigned long long int) * 8;
    }
    return __builtin_ctzll(value);
}

unsigned int stdc_trailing_ones_uc(unsigned char value)
{
    return stdc_trailing_zeros_uc(~value);
}

unsigned int stdc_trailing_ones_us(unsigned short value)
{
    return stdc_trailing_zeros_us(~value);
}

unsigned int stdc_trailing_ones_ui(unsigned int value)
{
    return stdc_trailing_zeros_ui(~value);
}

unsigned int stdc_trailing_ones_ul(unsigned long int value)
{
    return stdc_trailing_zeros_ul(~value);
}

unsigned int stdc_trailing_ones_ull(unsigned long long int value)
{
    return stdc_trailing_zeros_ull(~value);
}

unsigned int stdc_first_leading_zero_uc(unsigned char value)
{
    return sizeof(unsigned char) * 8 - stdc_leading_ones_uc(value);
}

unsigned int stdc_first_leading_zero_us(unsigned short value)
{
    return sizeof(unsigned short) * 8 - stdc_leading_ones_us(value);
}

unsigned int stdc_first_leading_zero_ui(unsigned int value)
{
    return sizeof(unsigned int) * 8 - stdc_leading_ones_ui(value);
}

unsigned int stdc_first_leading_zero_ul(unsigned long int value)
{
    return sizeof(unsigned long int) * 8 - stdc_leading_ones_ul(value);
}

unsigned int stdc_first_leading_zero_ull(unsigned long long int value)
{
    return sizeof(unsigned long long int) * 8 - stdc_leading_ones_ull(value);
}

unsigned int stdc_first_leading_one_uc(unsigned char value)
{
    return sizeof(unsigned char) * 8 - stdc_leading_zeros_uc(value);
}

unsigned int stdc_first_leading_one_us(unsigned short value)
{
    return sizeof(unsigned short) * 8 - stdc_leading_zeros_us(value);
}

unsigned int stdc_first_leading_one_ui(unsigned int value)
{
    return sizeof(unsigned int) * 8 - stdc_leading_zeros_ui(value);
}

unsigned int stdc_first_leading_one_ul(unsigned long int value)
{
    return sizeof(unsigned long int) * 8 - stdc_leading_zeros_ul(value);
}

unsigned int stdc_first_leading_one_ull(unsigned long long int value)
{
    return sizeof(unsigned long long int) * 8 - stdc_leading_zeros_ull(value);
}

unsigned int stdc_first_trailing_zero_uc(unsigned char value)
{
    return sizeof(unsigned char) * 8 - stdc_trailing_ones_uc(value);
}

unsigned int stdc_first_trailing_zero_us(unsigned short value)
{
    return sizeof(unsigned short) * 8 - stdc_trailing_ones_us(value);
}

unsigned int stdc_first_trailing_zero_ui(unsigned int value)
{
    return sizeof(unsigned int) * 8 - stdc_trailing_ones_ui(value);
}

unsigned int stdc_first_trailing_zero_ul(unsigned long int value)
{
    return sizeof(unsigned long int) * 8 - stdc_trailing_ones_ul(value);
}

unsigned int stdc_first_trailing_zero_ull(unsigned long long int value)
{
    return sizeof(unsigned long long int) * 8 - stdc_trailing_ones_ull(value);
}

unsigned int stdc_first_trailing_one_uc(unsigned char value)
{
    return sizeof(unsigned char) * 8 - stdc_trailing_zeros_uc(value);
}

unsigned int stdc_first_trailing_one_us(unsigned short value)
{
    return sizeof(unsigned short) * 8 - stdc_trailing_zeros_us(value);
}

unsigned int stdc_first_trailing_one_ui(unsigned int value)
{
    return sizeof(unsigned int) * 8 - stdc_trailing_zeros_ui(value);
}

unsigned int stdc_first_trailing_one_ul(unsigned long int value)
{
    return sizeof(unsigned long int) * 8 - stdc_trailing_zeros_ul(value);
}

unsigned int stdc_first_trailing_one_ull(unsigned long long int value)
{
    return sizeof(unsigned long long int) * 8 - stdc_trailing_zeros_ull(value);
}

unsigned int stdc_count_zeros_uc(unsigned char value)
{
    return stdc_count_ones_uc(~value);
}

unsigned int stdc_count_zeros_us(unsigned short value)
{
    return stdc_count_ones_us(~value);
}

unsigned int stdc_count_zeros_ui(unsigned int value)
{
    return stdc_count_ones_ui(~value);
}

unsigned int stdc_count_zeros_ul(unsigned long int value)
{
    return stdc_count_ones_ul(~value);
}

unsigned int stdc_count_zeros_ull(unsigned long long int value)
{
    return stdc_count_ones_ull(~value);
}

unsigned int stdc_count_ones_uc(unsigned char value)
{
    return __builtin_popcount(value);
}

unsigned int stdc_count_ones_us(unsigned short value)
{
    return __builtin_popcount(value);
}

unsigned int stdc_count_ones_ui(unsigned int value)
{
    return __builtin_popcount(value);
}

unsigned int stdc_count_ones_ul(unsigned long int value)
{
    return __builtin_popcountl(value);
}

unsigned int stdc_count_ones_ull(unsigned long long int value)
{
    return __builtin_popcountll(value);
}

bool stdc_has_single_bit_uc(unsigned char value)
{
    return (stdc_count_ones_uc(value) == 1);
}

bool stdc_has_single_bit_us(unsigned short value)
{
    return (stdc_count_ones_us(value) == 1);
}

bool stdc_has_single_bit_ui(unsigned int value)
{
    return (stdc_count_ones_ui(value) == 1);
}

bool stdc_has_single_bit_ul(unsigned long int value)
{
    return (stdc_count_ones_ul(value) == 1);
}

bool stdc_has_single_bit_ull(unsigned long long int value)
{
    return (stdc_count_ones_ull(value) == 1);
}

unsigned int stdc_bit_width_uc(unsigned char value)
{
    return sizeof(unsigned char) * 8 - stdc_leading_zeros_uc(value);
}

unsigned int stdc_bit_width_us(unsigned short value)
{
    return sizeof(unsigned short) * 8 - stdc_leading_zeros_us(value);
}

unsigned int stdc_bit_width_ui(unsigned int value)
{
    return sizeof(unsigned int) * 8 - stdc_leading_zeros_ui(value);
}

unsigned int stdc_bit_width_ul(unsigned long int value)
{
    return sizeof(unsigned long int) * 8 - stdc_leading_zeros_ul(value);
}

unsigned int stdc_bit_width_ull(unsigned long long int value)
{
    return sizeof(unsigned long long int) * 8 - stdc_leading_zeros_ull(value);
}

unsigned char stdc_bit_floor_uc(unsigned char value)
{
    if (value == 0) {
        return 0;
    }
    return 1 << (stdc_bit_width_uc(value) - 1);
}

unsigned short stdc_bit_floor_us(unsigned short value)
{
    if (value == 0) {
        return 0;
    }
    return 1 << (stdc_bit_width_us(value) - 1);
}

unsigned int stdc_bit_floor_ui(unsigned int value)
{
    if (value == 0) {
        return 0;
    }
    return 1 << (stdc_bit_width_ui(value) - 1);
}

unsigned long int stdc_bit_floor_ul(unsigned long int value)
{
    if (value == 0) {
        return 0;
    }
    return 1l << (stdc_bit_width_ul(value) - 1);
}

unsigned long long int stdc_bit_floor_ull(unsigned long long int value)
{
    if (value == 0) {
        return 0;
    }
    return 1ll << (stdc_bit_width_ull(value) - 1);
}

unsigned char stdc_bit_ceil_uc(unsigned char value)
{
    if (value <= 1) {
        return 1;
    } else if (stdc_leading_zeros_uc(value) == 0) {
        return 0;
    }
    return 1 << stdc_bit_width_uc(value - 1);
}

unsigned short stdc_bit_ceil_us(unsigned short value)
{
    if (value <= 1) {
        return 1;
    } else if (stdc_leading_zeros_us(value) == 0) {
        return 0;
    }
    return 1 << stdc_bit_width_us(value - 1);
}

unsigned int stdc_bit_ceil_ui(unsigned int value)
{
    if (value <= 1) {
        return 1;
    } else if (stdc_leading_zeros_ui(value) == 0) {
        return 0;
    }
    return 1 << stdc_bit_width_ui(value - 1);
}

unsigned long int stdc_bit_ceil_ul(unsigned long int value)
{
    if (value <= 1) {
        return 1;
    } else if (stdc_leading_zeros_ul(value) == 0) {
        return 0;
    }
    return 1 << stdc_bit_width_ul(value - 1);
}

unsigned long long int stdc_bit_ceil_ull(unsigned long long int value)
{
    if (value <= 1) {
        return 1;
    } else if (stdc_leading_zeros_ull(value) == 0) {
        return 0;
    }
    return 1 << stdc_bit_width_ull(value - 1);
}
