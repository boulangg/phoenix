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

struct gdt_desc
{
    std::uint16_t limit_0 : 16;
    std::uint16_t base_0  : 16;
    std::uint8_t base_1   : 8;
    std::uint8_t access   : 8;
    std::uint8_t limit_1  : 4;
    std::uint8_t flags    : 4;
    std::uint8_t base_2   : 8;
} __attribute__((packed));
static_assert(sizeof(struct gdt_desc) == 8, "gdt_desc size incorrect");

struct gdt_desc_ext
{
    gdt_desc gate_base;
    std::uint32_t base_3   : 32;
    std::uint32_t reserved : 32;
} __attribute__((packed));
static_assert(sizeof(struct gdt_desc_ext) == 16, "gdt_desc_ext size incorrect");

extern gdt_desc gdt[GDT_ENTRIES];

void setupGDT();

};

namespace TSS {

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
};
}

#ifdef __cplusplus
extern "C"
{
#endif

// ASM functions
extern void set_GDT(std::uint16_t size, kernel::GDT::gdt_desc* base);
extern void set_TSS(std::uint16_t selector);

#ifdef __cplusplus
}
#endif
