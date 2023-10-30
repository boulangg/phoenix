/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include <cstddef>
#include <cstdint>

namespace kernel::core {

namespace IDT {
static constexpr std::size_t IDT_ENTRIES = 256;

static constexpr std::uint8_t ACCESS_P = 0x80;
static constexpr std::uint8_t ACCESS_DPL0 = 0x00;
static constexpr std::uint8_t ACCESS_DPL1 = 0x20;
static constexpr std::uint8_t ACCESS_DPL2 = 0x40;
static constexpr std::uint8_t ACCESS_DPL3 = 0x60;

static constexpr std::uint8_t GATE_TYPE_INTERRUPT = 0b1110;
static constexpr std::uint8_t GATE_TYPE_TRAP = 0b1111;

// IDT
struct idt_desc
{
    std::uint16_t offset_1   : 16;
    std::uint16_t selector   : 16;
    std::uint8_t ist         : 3;
    std::uint8_t reserved_1 : 5;
    std::uint8_t flags       : 8;
    std::uint16_t offset_2   : 16;
    std::uint32_t offset_3   : 32;
    std::uint32_t reserved_2 : 32;
} __attribute__((packed));
static_assert(sizeof(idt_desc) == 16, "idt_desc size incorrect");

extern idt_desc idt[IDT_ENTRIES];
static constexpr std::size_t IDT_SIZE = sizeof(idt_desc) * IDT_ENTRIES - 1;

void setupIDT();

}

}

// ASM functions
#ifdef __cplusplus
extern "C"
{
#endif

extern void set_IDT(std::uint16_t size, kernel::core::IDT::idt_desc* base);

#ifdef __cplusplus
}
#endif
