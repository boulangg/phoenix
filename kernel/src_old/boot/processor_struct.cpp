/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "processor_struct.hpp"
#include <include/constant.h>

// static_assert(sizeof(struct page_entry) == 8, "page_entry size incorrect");
uint64_t kernel_pml4t[512] __attribute__((aligned(4096)));
uint32_t kernel_page_limit;

static_assert(sizeof(struct gate_desc) == 16, "gate_desc size incorrect");
struct gate_desc idt[IDT_ENTRIES] __attribute__((aligned(4096)));

uint64_t gdt[GDT_ENTRIES] __attribute__((aligned(4096)));

struct x86_64_tss tss __attribute__((aligned(4096)));

uint32_t multiboot_info_tags[MULTIBOOT_INFO_SIZE] __attribute__((aligned(4096)));
