/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <cstdint>

#define GDT_ENTRIES 512
#define IDT_ENTRIES 256

#define GDT_SIZE (GDT_ENTRIES * 8 - 1)
#define IDT_SIZE (IDT_ENTRIES * 16 - 1)

#define PAGE_SIZE 0x1000

#define BASE_PAGE_ARRAY_MAPPING (2ul << 16) // 8 Mo

// GDT Selector
namespace kernel::processor {

struct GDT
{
    struct Access
    {
        static constexpr std::uint8_t ACCESS_P = 0x80;
        static constexpr std::uint8_t ACCESS_A = 0x01;
        static constexpr std::uint8_t ACCESS_PA = ACCESS_P | ACCESS_A;
        static constexpr std::uint8_t ACCESS_DPL0 = 0x00;
        static constexpr std::uint8_t ACCESS_DPL1 = 0x20;
        static constexpr std::uint8_t ACCESS_DPL2 = 0x40;
        static constexpr std::uint8_t ACCESS_DPL3 = 0x60;

        static constexpr std::uint8_t ACCESS_TYPE_SYS = 0x00;
        static constexpr std::uint8_t ACCESS_TYPE_DATA = 0x10;
        static constexpr std::uint8_t ACCESS_TYPE_CODE = 0x18;

        struct Data
        {
            static constexpr std::uint8_t ACCESS_DIR_DOWN = 0x04;

            static constexpr std::uint8_t ACCESS_WRITE_ALLOWED = 0x02;
        };

        struct Code
        {
            static constexpr std::uint8_t ACCESS_CONFORM = 0x04;

            static constexpr std::uint8_t ACCESS_READ_ALLOWED = 0x02;
        };
    };

    struct Flags
    {
        static constexpr std::uint8_t FLAG_GRANULARITY = 0x08;
        static constexpr std::uint8_t FLAG_SIZE_16 = 0x04;
        static constexpr std::uint8_t FLAG_SIZE_32 = 0x00;
        static constexpr std::uint8_t FLAG_SIZE_64 = 0x02;
    };

    static constexpr std::uint8_t KERNEL_NULL_OFFSET = 0x00;
    static constexpr std::uint8_t KERNEL_CS_16_OFFSET = 0x08;
    static constexpr std::uint8_t KERNEL_DS_16_OFFSET = 0x10;
    static constexpr std::uint8_t KERNEL_CS_32_OFFSET = 0x18;
    static constexpr std::uint8_t KERNEL_DS_32_OFFSET = 0x20;
    static constexpr std::uint8_t KERNEL_CS_64_OFFSET = 0x28;
    static constexpr std::uint8_t KERNEL_DS_64_OFFSET = 0x30;
    static constexpr std::uint8_t USER_CS_64_OFFSET = 0x38;
    static constexpr std::uint8_t USER_DS_64_OFFSET = 0x40;

    static constexpr std::uint8_t KERNEL_NULL_INDEX = KERNEL_NULL_OFFSET >> 3;

    static constexpr std::uint16_t KERNEL_CS_16_INDEX = KERNEL_CS_16_OFFSET >> 3;
    static constexpr std::uint16_t KERNEL_CS_16_BASE = 0x00;
    static constexpr std::uint16_t KERNEL_CS_16_LIMIT = 0xffff;
    static constexpr std::uint8_t KERNEL_CS_16_FLAGS = Flags::FLAG_SIZE_16;
    static constexpr std::uint8_t KERNEL_CS_16_ACCESS =
        Access::ACCESS_PA | Access::ACCESS_DPL0 | Access::ACCESS_TYPE_CODE | Access::Code::ACCESS_READ_ALLOWED;

    static constexpr std::uint16_t KERNEL_DS_16_INDEX = KERNEL_DS_16_OFFSET >> 3;
    static constexpr std::uint16_t KERNEL_DS_16_BASE = 0x00;
    static constexpr std::uint16_t KERNEL_DS_16_LIMIT = 0xffff;
    static constexpr std::uint8_t KERNEL_DS_16_FLAGS = Flags::FLAG_SIZE_16;
    static constexpr std::uint8_t KERNEL_DS_16_ACCESS =
        Access::ACCESS_PA | Access::ACCESS_DPL0 | Access::ACCESS_TYPE_DATA | Access::Data::ACCESS_WRITE_ALLOWED;

    static constexpr std::uint16_t KERNEL_CS_32_INDEX = KERNEL_CS_32_OFFSET >> 3;
    static constexpr std::uint32_t KERNEL_CS_32_BASE = 0x00;
    static constexpr std::uint32_t KERNEL_CS_32_LIMIT = 0xffffffff;
    static constexpr std::uint8_t KERNEL_CS_32_FLAGS = Flags::FLAG_SIZE_32;
    static constexpr std::uint8_t KERNEL_CS_32_ACCESS =
        Access::ACCESS_PA | Access::ACCESS_DPL0 | Access::ACCESS_TYPE_CODE | Access::Code::ACCESS_READ_ALLOWED;

    static constexpr std::uint32_t KERNEL_DS_32_INDEX = KERNEL_DS_32_OFFSET >> 3;
    static constexpr std::uint32_t KERNEL_DS_32_BASE = 0x00;
    static constexpr std::uint32_t KERNEL_DS_32_LIMIT = 0xffffffff;
    static constexpr std::uint8_t KERNEL_DS_32_FLAGS = Flags::FLAG_SIZE_32;
    static constexpr std::uint8_t KERNEL_DS_32_ACCESS =
        Access::ACCESS_PA | Access::ACCESS_DPL0 | Access::ACCESS_TYPE_DATA | Access::Data::ACCESS_WRITE_ALLOWED;

    static constexpr std::uint16_t KERNEL_CS_64_INDEX = KERNEL_CS_64_OFFSET >> 3;
    static constexpr std::uint64_t KERNEL_CS_64_BASE = 0x00;
    static constexpr std::uint64_t KERNEL_CS_64_LIMIT = 0x00;
    static constexpr std::uint8_t KERNEL_CS_64_FLAGS = Flags::FLAG_SIZE_64;
    static constexpr std::uint8_t KERNEL_CS_64_ACCESS =
        Access::ACCESS_PA | Access::ACCESS_DPL0 | Access::ACCESS_TYPE_CODE | Access::Code::ACCESS_READ_ALLOWED;

    static constexpr std::uint64_t KERNEL_DS_64_INDEX = KERNEL_DS_64_OFFSET >> 3;
    static constexpr std::uint64_t KERNEL_DS_64_BASE = 0x00;
    static constexpr std::uint64_t KERNEL_DS_64_LIMIT = 0x00;
    static constexpr std::uint8_t KERNEL_DS_64_FLAGS = Flags::FLAG_SIZE_64;
    static constexpr std::uint8_t KERNEL_DS_64_ACCESS =
        Access::ACCESS_PA | Access::ACCESS_DPL0 | Access::ACCESS_TYPE_DATA | Access::Data::ACCESS_WRITE_ALLOWED;
    static constexpr std::uint16_t USER_CS_64_INDEX = USER_CS_64_OFFSET >> 3;
    static constexpr std::uint64_t USER_CS_64_BASE = 0x00;
    static constexpr std::uint64_t USER_CS_64_LIMIT = 0x00;
    static constexpr std::uint8_t USER_CS_64_FLAGS = Flags::FLAG_SIZE_64;
    static constexpr std::uint8_t USER_CS_64_ACCESS =
        Access::ACCESS_PA | Access::ACCESS_DPL3 | Access::ACCESS_TYPE_CODE | Access::Code::ACCESS_READ_ALLOWED;

    static constexpr std::uint64_t USER_DS_64_INDEX = USER_DS_64_OFFSET >> 3;
    static constexpr std::uint64_t USER_DS_64_BASE = 0x00;
    static constexpr std::uint64_t USER_DS_64_LIMIT = 0x00;
    static constexpr std::uint8_t USER_DS_64_FLAGS = Flags::FLAG_SIZE_64;
    static constexpr std::uint8_t USER_DS_64_ACCESS =
        Access::ACCESS_PA | Access::ACCESS_DPL3 | Access::ACCESS_TYPE_DATA | Access::Data::ACCESS_WRITE_ALLOWED;
};

}
