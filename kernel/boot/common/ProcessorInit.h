/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <cstddef>
#include <cstdint>

#include "ProcessorContant.h"

#ifdef __cplusplus
extern "C"
{
#endif

// GDT
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
extern gdt_desc gdt[GDT_ENTRIES];
extern void set_GDT(std::uint16_t size, gdt_desc* base);

// IDT
struct idt_desc
{
    std::uint64_t offset_1   : 16;
    std::uint16_t selector   : 16;
    std::uint8_t ist         : 3;
    std::uint64_t reserved_1 : 5;
    std::uint8_t flags       : 8;
    std::uint64_t offset_2   : 16;
    std::uint64_t offset_3   : 32;
    std::uint64_t reserved_2 : 32;
} __attribute__((packed));
static_assert(sizeof(struct idt_desc) == 16, "idt_desc size incorrect");
extern idt_desc idt[IDT_ENTRIES];
extern void set_IDT(std::uint16_t size, struct idt_desc* base);

#ifdef __cplusplus
}
#endif

namespace kernel::processor {

void init();

}
