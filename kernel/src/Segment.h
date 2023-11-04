/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <cstdint>

// GDT Selector
namespace kernel {

namespace GDT {
static constexpr std::uint64_t GDT_ENTRIES = 512;
static constexpr std::uint64_t GDT_SIZE = (GDT_ENTRIES * 8 - 1);

struct gdt_desc
{
    std::uint16_t limit_0 : 16;
    std::uint16_t base_0  : 16;
    std::uint8_t base_1   : 8;
    std::uint8_t access   : 8;
    std::uint8_t limit_1  : 4;
    std::uint8_t flags    : 4;
    std::uint8_t base_2   : 8;
};
static_assert(sizeof(struct gdt_desc) == 8, "gdt_desc size incorrect");

struct gdt_desc_ext
{
    gdt_desc gate_base;
    std::uint32_t base_3   : 32;
    std::uint32_t reserved : 32;
};
static_assert(sizeof(struct gdt_desc_ext) == 16, "gdt_desc_ext size incorrect");

extern gdt_desc gdt[GDT_ENTRIES];

void setupGDT();

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

    struct System
    {
        static constexpr std::uint8_t ACCESS_TYPE_LDT = 0x02;
        static constexpr std::uint8_t ACCESS_TYPE_64_AVAIL = 0x09;
        static constexpr std::uint8_t ACCESS_TYPE_64_BUSY = 0x0B;
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
static constexpr std::uint8_t TSS_OFFSET = 0x50;

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
static constexpr std::uint32_t KERNEL_CS_64_BASE = 0x00;
static constexpr std::uint32_t KERNEL_CS_64_LIMIT = 0x00;
static constexpr std::uint8_t KERNEL_CS_64_FLAGS = Flags::FLAG_SIZE_64;
static constexpr std::uint8_t KERNEL_CS_64_ACCESS =
    Access::ACCESS_PA | Access::ACCESS_DPL0 | Access::ACCESS_TYPE_CODE | Access::Code::ACCESS_READ_ALLOWED;

static constexpr std::uint64_t KERNEL_DS_64_INDEX = KERNEL_DS_64_OFFSET >> 3;
static constexpr std::uint32_t KERNEL_DS_64_BASE = 0x00;
static constexpr std::uint32_t KERNEL_DS_64_LIMIT = 0x00;
static constexpr std::uint8_t KERNEL_DS_64_FLAGS = Flags::FLAG_SIZE_64;
static constexpr std::uint8_t KERNEL_DS_64_ACCESS =
    Access::ACCESS_PA | Access::ACCESS_DPL0 | Access::ACCESS_TYPE_DATA | Access::Data::ACCESS_WRITE_ALLOWED;
static constexpr std::uint16_t USER_CS_64_INDEX = USER_CS_64_OFFSET >> 3;
static constexpr std::uint32_t USER_CS_64_BASE = 0x00;
static constexpr std::uint32_t USER_CS_64_LIMIT = 0x00;
static constexpr std::uint8_t USER_CS_64_FLAGS = Flags::FLAG_SIZE_64;
static constexpr std::uint8_t USER_CS_64_ACCESS =
    Access::ACCESS_PA | Access::ACCESS_DPL3 | Access::ACCESS_TYPE_CODE | Access::Code::ACCESS_READ_ALLOWED;

static constexpr std::uint64_t USER_DS_64_INDEX = USER_DS_64_OFFSET >> 3;
static constexpr std::uint32_t USER_DS_64_BASE = 0x00;
static constexpr std::uint32_t USER_DS_64_LIMIT = 0x00;
static constexpr std::uint8_t USER_DS_64_FLAGS = Flags::FLAG_SIZE_64;
static constexpr std::uint8_t USER_DS_64_ACCESS =
    Access::ACCESS_PA | Access::ACCESS_DPL3 | Access::ACCESS_TYPE_DATA | Access::Data::ACCESS_WRITE_ALLOWED;
};

namespace TSS {
static constexpr std::uint64_t KERNEL_STACK_TOP = 0xFFFF840000010000;

struct tss_desc
{
    uint32_t reserved_1 : 32;
    uint64_t* rsp0; /* stack pointer for ring 0 */
    uint64_t* rsp1; /* stack pointer for ring 1 */
    uint64_t* rsp2; /* stack pointer for ring 2 */
    uint64_t reserved_2;
    uint64_t* ist1; /* interrupt stack pointer 1 */
    uint64_t* ist2; /* interrupt stack pointer 2 */
    uint64_t* ist3; /* interrupt stack pointer 3 */
    uint64_t* ist4; /* interrupt stack pointer 4 */
    uint64_t* ist5; /* interrupt stack pointer 5 */
    uint64_t* ist6; /* interrupt stack pointer 6 */
    uint64_t* ist7; /* interrupt stack pointer 7 */
    uint64_t reserved_3;
    uint16_t reserved_4;
    uint16_t io_bit_map_offset; /* offset to start of IO permission bit map */
} __attribute__((packed));
static_assert(sizeof(struct tss_desc) == 104, "x86_64_tss size incorrect");

extern tss_desc tss;

void setupTSS();

static constexpr std::uint64_t TSS_INDEX = GDT::TSS_OFFSET >> 3;
static constexpr std::uint32_t TSS_LIMIT = sizeof(tss);
static constexpr std::uint8_t TSS_FLAGS = 0;
static constexpr std::uint8_t TSS_ACCESS = (GDT::Access::ACCESS_P | GDT::Access::ACCESS_DPL0 |
                                            GDT::Access::ACCESS_TYPE_SYS | GDT::Access::System::ACCESS_TYPE_64_AVAIL);
};

}

// ASM functions
#ifdef __cplusplus
extern "C"
{
#endif

extern void set_GDT(std::uint16_t size, kernel::GDT::gdt_desc* base);
extern void set_TSS(std::uint16_t selector);

#ifdef __cplusplus
}
#endif
