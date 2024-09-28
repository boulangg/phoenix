/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 
#include "UnitTestFramework.h"

#include <stdbit.h>
#include <unordered_map>

using namespace UnitTestFramework;

namespace libc {

TEST_CLASS(stdbit)
{
public:
    TEST_METHOD(stdc_leading_zeros_test)
    {
        // unsigned char
        std::unordered_map<unsigned char, unsigned int> test_result_uc;
        test_result_uc.insert(std::make_pair(0x00, 8u));
        test_result_uc.insert(std::make_pair(0x08, 4u));
        test_result_uc.insert(std::make_pair(0xA0, 0u));

        for (auto& item : test_result_uc) {
            Assert::AreEqual(item.second, stdc_leading_zeros_uc(item.first));
        }

        // unsigned short
        std::unordered_map<unsigned short, unsigned int> test_result_us;
        test_result_us.insert(std::make_pair(0x0000, 16u));
        test_result_us.insert(std::make_pair(0x0800, 4u));
        test_result_us.insert(std::make_pair(0xA000, 0u));

        for (auto& item : test_result_us) {
            Assert::AreEqual(item.second, stdc_leading_zeros_us(item.first));
        }

        // unsigned int
        std::unordered_map<unsigned int, unsigned int> test_result_ui;
        test_result_ui.insert(std::make_pair(0x00000000, 32u));
        test_result_ui.insert(std::make_pair(0x08000000, 4u));
        test_result_ui.insert(std::make_pair(0xA0000000, 0u));

        for (auto& item : test_result_ui) {
            Assert::AreEqual(item.second, stdc_leading_zeros_ui(item.first));
        }

        // macro
        unsigned char val_char = 0x08;
        Assert::AreEqual(4ul, stdc_leading_zeros(val_char));
        unsigned short val_shr = 0x08;
        Assert::AreEqual(12ul, stdc_leading_zeros(val_shr));
        unsigned int val_int = 0x08;
        Assert::AreEqual(28ul, stdc_leading_zeros(val_int));
        unsigned long long val_long_long = 0x08;
        Assert::AreEqual(60ul, stdc_leading_zeros(val_long_long));
    }

    TEST_METHOD(stdc_leading_ones_test)
    {
        // unsigned char
        std::unordered_map<unsigned char, unsigned int> test_result_uc;
        test_result_uc.insert(std::make_pair(0x00, 0u));
        test_result_uc.insert(std::make_pair(0x80, 1u));
        test_result_uc.insert(std::make_pair(0xE0, 3u));

        for (auto& item : test_result_uc) {
            Assert::AreEqual(item.second, stdc_leading_ones_uc(item.first));
        }

        // unsigned short
        std::unordered_map<unsigned short, unsigned int> test_result_us;
        test_result_us.insert(std::make_pair(0x0000, 0u));
        test_result_us.insert(std::make_pair(0x8000, 1u));
        test_result_us.insert(std::make_pair(0xE000, 3u));

        for (auto& item : test_result_us) {
            Assert::AreEqual(item.second, stdc_leading_ones_us(item.first));
        }

        // unsigned int
        std::unordered_map<unsigned int, unsigned int> test_result_ui;
        test_result_ui.insert(std::make_pair(0x00000000, 0u));
        test_result_ui.insert(std::make_pair(0x80000000, 1u));
        test_result_ui.insert(std::make_pair(0xE0000000, 3u));

        for (auto& item : test_result_ui) {
            Assert::AreEqual(item.second, stdc_leading_ones_ui(item.first));
        }

        // macro
        unsigned char val_char = 0xF0;
        Assert::AreEqual(4ul, stdc_leading_ones(val_char));
        unsigned short val_shr = 0xF000;
        Assert::AreEqual(4ul, stdc_leading_ones(val_shr));
        unsigned int val_int = 0xF0000000;
        Assert::AreEqual(4ul, stdc_leading_ones(val_int));
        unsigned long long val_long_long = 0xF000000000000000;
        Assert::AreEqual(4ul, stdc_leading_ones(val_long_long));
    }

    TEST_METHOD(stdc_trailing_zeros_test)
    {
        // unsigned char
        std::unordered_map<unsigned char, unsigned int> test_result_uc;
        test_result_uc.insert(std::make_pair(0x00, 8u));
        test_result_uc.insert(std::make_pair(0x01, 0u));
        test_result_uc.insert(std::make_pair(0x10, 4u));
        test_result_uc.insert(std::make_pair(0x80, 7u));

        for (auto& item : test_result_uc) {
            Assert::AreEqual(item.second, stdc_trailing_zeros_uc(item.first));
        }

        // unsigned short
        std::unordered_map<unsigned short, unsigned int> test_result_us;
        test_result_us.insert(std::make_pair(0x0000, 16u));
        test_result_us.insert(std::make_pair(0x0001, 0u));
        test_result_us.insert(std::make_pair(0x0100, 8u));
        test_result_us.insert(std::make_pair(0x8000, 15u));

        for (auto& item : test_result_us) {
            Assert::AreEqual(item.second, stdc_trailing_zeros_us(item.first));
        }

        // unsigned int
        std::unordered_map<unsigned int, unsigned int> test_result_ui;
        test_result_ui.insert(std::make_pair(0x00000000, 32u));
        test_result_ui.insert(std::make_pair(0x00000001, 0u));
        test_result_ui.insert(std::make_pair(0x01000000, 24u));
        test_result_ui.insert(std::make_pair(0x80000000, 31u));

        for (auto& item : test_result_ui) {
            Assert::AreEqual(item.second, stdc_trailing_zeros_ui(item.first));
        }

        // macro
        unsigned char val_char = 0x00;
        Assert::AreEqual(8ul, stdc_trailing_zeros(val_char));
        unsigned short val_shr = 0x00;
        Assert::AreEqual(16ul, stdc_trailing_zeros(val_shr));
        unsigned int val_int = 0x00;
        Assert::AreEqual(32ul, stdc_trailing_zeros(val_int));
        unsigned long long val_long_long = 0x1000000000000000;
        Assert::AreEqual(60ul, stdc_trailing_zeros(val_long_long));
    }

    TEST_METHOD(stdc_trailing_ones_test)
    {
        // unsigned char
        std::unordered_map<unsigned char, unsigned int> test_result_uc;
        test_result_uc.insert(std::make_pair(0x00, 0u));
        test_result_uc.insert(std::make_pair(0x01, 1u));
        test_result_uc.insert(std::make_pair(0x1F, 5u));
        test_result_uc.insert(std::make_pair(0xFF, 8u));

        for (auto& item : test_result_uc) {
            Assert::AreEqual(item.second, stdc_trailing_ones_uc(item.first));
        }

        // unsigned short
        std::unordered_map<unsigned short, unsigned int> test_result_us;
        test_result_us.insert(std::make_pair(0x0000, 0u));
        test_result_uc.insert(std::make_pair(0x0001, 1u));
        test_result_us.insert(std::make_pair(0x1FFF, 13u));
        test_result_us.insert(std::make_pair(0xFFFF, 16u));

        for (auto& item : test_result_us) {
            Assert::AreEqual(item.second, stdc_trailing_ones_us(item.first));
        }

        // unsigned int
        std::unordered_map<unsigned int, unsigned int> test_result_ui;
        test_result_ui.insert(std::make_pair(0x00000000, 0u));
        test_result_ui.insert(std::make_pair(0x00000001, 1u));
        test_result_ui.insert(std::make_pair(0x1FFFFFFF, 29u));
        test_result_ui.insert(std::make_pair(0xFFFFFFFF, 32u));

        for (auto& item : test_result_ui) {
            Assert::AreEqual(item.second, stdc_trailing_ones_ui(item.first));
        }

        // macro
        unsigned char val_char = 0xFF;
        Assert::AreEqual(8ul, stdc_trailing_ones(val_char));
        unsigned short val_shr = 0xFFFF;
        Assert::AreEqual(16ul, stdc_trailing_ones(val_shr));
        unsigned int val_int = 0xFFFFFFFF;
        Assert::AreEqual(32ul, stdc_trailing_ones(val_int));
        unsigned long long val_long_long = 0x1FFFFFFFFFFFFFFF;
        Assert::AreEqual(61ul, stdc_trailing_ones(val_long_long));
    }

    TEST_METHOD(stdc_count_zeros_test)
    {
        // unsigned char
        std::unordered_map<unsigned char, unsigned int> test_result_uc;
        test_result_uc.insert(std::make_pair(0x00, 8u));
        test_result_uc.insert(std::make_pair(0x80, 7u));
        test_result_uc.insert(std::make_pair(0xFF, 0u));

        for (auto& item : test_result_uc) {
            Assert::AreEqual(item.second, stdc_count_zeros_uc(item.first));
        }

        // unsigned short
        std::unordered_map<unsigned short, unsigned int> test_result_us;
        test_result_us.insert(std::make_pair(0x0000, 16u));
        test_result_us.insert(std::make_pair(0x8000, 15u));
        test_result_us.insert(std::make_pair(0xFFFF, 0u));

        for (auto& item : test_result_us) {
            Assert::AreEqual(item.second, stdc_count_zeros_us(item.first));
        }

        // unsigned int
        std::unordered_map<unsigned int, unsigned int> test_result_ui;
        test_result_ui.insert(std::make_pair(0x00000000, 32u));
        test_result_ui.insert(std::make_pair(0x80000000, 31u));
        test_result_ui.insert(std::make_pair(0xFFFFFFFF, 0u));

        for (auto& item : test_result_ui) {
            Assert::AreEqual(item.second, stdc_count_zeros_ui(item.first));
        }

        // macro
        unsigned char val_char = 0x00;
        Assert::AreEqual(8ul, stdc_count_zeros(val_char));
        unsigned short val_shr = 0x00;
        Assert::AreEqual(16ul, stdc_count_zeros(val_shr));
        unsigned int val_int = 0x00;
        Assert::AreEqual(32ul, stdc_count_zeros(val_int));
        unsigned long long val_long_long = 0x0F00000000000000;
        Assert::AreEqual(60ul, stdc_count_zeros(val_long_long));
    }

    TEST_METHOD(stdc_count_ones_test)
    {
        // unsigned char
        std::unordered_map<unsigned char, unsigned int> test_result_uc;
        test_result_uc.insert(std::make_pair(0x00, 0u));
        test_result_uc.insert(std::make_pair(0x80, 1u));
        test_result_uc.insert(std::make_pair(0xE0, 3u));

        for (auto& item : test_result_uc) {
            Assert::AreEqual(item.second, stdc_count_ones_uc(item.first));
        }

        // unsigned short
        std::unordered_map<unsigned short, unsigned int> test_result_us;
        test_result_us.insert(std::make_pair(0x0000, 0u));
        test_result_us.insert(std::make_pair(0x8000, 1u));
        test_result_us.insert(std::make_pair(0xE000, 3u));

        for (auto& item : test_result_us) {
            Assert::AreEqual(item.second, stdc_count_ones_us(item.first));
        }

        // unsigned int
        std::unordered_map<unsigned int, unsigned int> test_result_ui;
        test_result_ui.insert(std::make_pair(0x00000000, 0u));
        test_result_ui.insert(std::make_pair(0x80000000, 1u));
        test_result_ui.insert(std::make_pair(0xE0000000, 3u));

        for (auto& item : test_result_ui) {
            Assert::AreEqual(item.second, stdc_count_ones_ui(item.first));
        }

        // macro
        unsigned char val_char = 0xFF;
        Assert::AreEqual(8u, stdc_count_ones(val_char));
        unsigned short val_shr = 0xFFFF;
        Assert::AreEqual(16u, stdc_count_ones(val_shr));
        unsigned int val_int = 0xF0000000;
        Assert::AreEqual(4u, stdc_count_ones(val_int));
        unsigned long long val_long_long = 0x0F00000000000000;
        Assert::AreEqual(4u, stdc_count_ones(val_long_long));
    }

    TEST_METHOD(stdc_has_single_bit_test)
    {
        // unsigned char
        std::unordered_map<unsigned char, bool> test_result_uc;
        test_result_uc.insert(std::make_pair(0x00, false));
        test_result_uc.insert(std::make_pair(0x80, true));
        test_result_uc.insert(std::make_pair(0x81, false));

        for (auto& item : test_result_uc) {
            Assert::AreEqual(item.second, stdc_has_single_bit_uc(item.first));
        }

        // unsigned short
        std::unordered_map<unsigned short, bool> test_result_us;
        test_result_us.insert(std::make_pair(0x0000, false));
        test_result_us.insert(std::make_pair(0x8000, true));
        test_result_us.insert(std::make_pair(0xE000, false));

        for (auto& item : test_result_us) {
            Assert::AreEqual(item.second, stdc_has_single_bit_us(item.first));
        }

        // unsigned int
        std::unordered_map<unsigned int, bool> test_result_ui;
        test_result_ui.insert(std::make_pair(0x00000000, false));
        test_result_ui.insert(std::make_pair(0x80000000, true));
        test_result_ui.insert(std::make_pair(0xE0000000, false));

        for (auto& item : test_result_ui) {
            Assert::AreEqual(item.second, stdc_has_single_bit_ui(item.first));
        }

        // macro
        unsigned char val_char = 0x80;
        Assert::AreEqual(true, stdc_has_single_bit(val_char));
        unsigned short val_shr = 0x8000;
        Assert::AreEqual(true, stdc_has_single_bit(val_shr));
        unsigned int val_int = 0x80000000;
        Assert::AreEqual(true, stdc_has_single_bit(val_int));
        unsigned long long val_long_long = 0xF000000000000000;
        Assert::AreEqual(false, stdc_has_single_bit(val_long_long));
    }

    TEST_METHOD(stdc_bit_width_test)
    {
        // unsigned char
        std::unordered_map<unsigned char, unsigned int> test_result_uc;
        test_result_uc.insert(std::make_pair(0x00, 0u));
        test_result_uc.insert(std::make_pair(0x40, 7u));
        test_result_uc.insert(std::make_pair(0xE0, 8u));

        for (auto& item : test_result_uc) {
            Assert::AreEqual(item.second, stdc_bit_width_uc(item.first));
        }

        // unsigned short
        std::unordered_map<unsigned short, unsigned int> test_result_us;
        test_result_us.insert(std::make_pair(0x0000, 0u));
        test_result_us.insert(std::make_pair(0x4000, 15u));
        test_result_us.insert(std::make_pair(0xE000, 16u));

        for (auto& item : test_result_us) {
            Assert::AreEqual(item.second, stdc_bit_width_us(item.first));
        }

        // unsigned int
        std::unordered_map<unsigned int, unsigned int> test_result_ui;
        test_result_ui.insert(std::make_pair(0x00000000, 0u));
        test_result_ui.insert(std::make_pair(0x40000000, 31u));
        test_result_ui.insert(std::make_pair(0xE0000000, 32u));

        for (auto& item : test_result_ui) {
            Assert::AreEqual(item.second, stdc_bit_width_ui(item.first));
        }

        // macro
        unsigned char val_char = 0xF0;
        Assert::AreEqual(8u, stdc_bit_width(val_char));
        unsigned short val_shr = 0xF000;
        Assert::AreEqual(16u, stdc_bit_width(val_shr));
        unsigned int val_int = 0xF0000000;
        Assert::AreEqual(32u, stdc_bit_width(val_int));
        unsigned long long val_long_long = 0xF000000000000000;
        Assert::AreEqual(64u, stdc_bit_width(val_long_long));
    }

    TEST_METHOD(stdc_bit_floor_test)
    {
        // unsigned char
        std::unordered_map<unsigned char, unsigned char> test_result_uc;
        test_result_uc.insert(std::make_pair(0x00, 0x00));
        test_result_uc.insert(std::make_pair(0x40, 0x40));
        test_result_uc.insert(std::make_pair(0xE0, 0x80));

        for (auto& item : test_result_uc) {
            Assert::AreEqual(item.second, stdc_bit_floor_uc(item.first));
        }

        // unsigned short
        std::unordered_map<unsigned short, unsigned short> test_result_us;
        test_result_us.insert(std::make_pair(0x0000, 0x0000));
        test_result_us.insert(std::make_pair(0x4000, 0x4000));
        test_result_us.insert(std::make_pair(0xE000, 0x8000));

        for (auto& item : test_result_us) {
            Assert::AreEqual(item.second, stdc_bit_floor_us(item.first));
        }

        // unsigned int
        std::unordered_map<unsigned int, unsigned int> test_result_ui;
        test_result_ui.insert(std::make_pair(0x00000000, 0x00000000));
        test_result_ui.insert(std::make_pair(0x40000000, 0x40000000));
        test_result_ui.insert(std::make_pair(0xE0000000, 0x80000000));

        for (auto& item : test_result_ui) {
            Assert::AreEqual(item.second, stdc_bit_floor_ui(item.first));
        }

        // macro
        unsigned char val_char = 0xF0;
        Assert::AreEqual(0x80ull, stdc_bit_floor(val_char));
        unsigned short val_shr = 0xF000;
        Assert::AreEqual(0x8000ull, stdc_bit_floor(val_shr));
        unsigned int val_int = 0xF0000000;
        Assert::AreEqual(0x80000000ull, stdc_bit_floor(val_int));
        unsigned long long val_long_long = 0xF000000000000000;
        Assert::AreEqual(0x8000000000000000ull, stdc_bit_floor(val_long_long));
    }

    TEST_METHOD(stdc_bit_ceil_test)
    {
        // unsigned char
        std::unordered_map<unsigned char, unsigned char> test_result_uc;
        test_result_uc.insert(std::make_pair(0x00, 0x01));
        test_result_uc.insert(std::make_pair(0x40, 0x40));
        test_result_uc.insert(std::make_pair(0xFF, 0x00));

        for (auto& item : test_result_uc) {
            Assert::AreEqual(item.second, stdc_bit_ceil_uc(item.first));
        }

        // unsigned short
        std::unordered_map<unsigned short, unsigned short> test_result_us;
        test_result_us.insert(std::make_pair(0x0000, 0x0001));
        test_result_us.insert(std::make_pair(0x4000, 0x4000));
        test_result_us.insert(std::make_pair(0xE000, 0x00));

        for (auto& item : test_result_us) {
            Assert::AreEqual(item.second, stdc_bit_ceil_us(item.first));
        }

        // unsigned int
        std::unordered_map<unsigned int, unsigned int> test_result_ui;
        test_result_ui.insert(std::make_pair(0x00000000, 0x00000001));
        test_result_ui.insert(std::make_pair(0x40000000, 0x40000000));
        test_result_ui.insert(std::make_pair(0xE0000000, 0x00));

        for (auto& item : test_result_ui) {
            Assert::AreEqual(item.second, stdc_bit_ceil_ui(item.first));
        }

        // macro
        unsigned char val_char = 0xF0;
        Assert::AreEqual(0x00, stdc_bit_ceil(val_char));
        unsigned short val_shr = 0xF000;
        Assert::AreEqual(0x00, stdc_bit_ceil(val_shr));
        unsigned int val_int = 0xF0000000;
        Assert::AreEqual(0x00, stdc_bit_ceil(val_int));
        unsigned long long val_long_long = 0xF000000000000000;
        Assert::AreEqual(0x00, stdc_bit_ceil(val_long_long));
    }
};

}
