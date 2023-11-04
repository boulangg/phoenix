/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #include "Segment.h"

namespace kernel {

GDT::gdt_desc GDT::gdt[GDT::GDT_ENTRIES] __attribute__((aligned(4096)));
TSS::tss_desc TSS::tss;

static void fill_segment_descriptor(std::uint8_t index, std::uint64_t base, std::uint32_t limit, std::uint8_t flags,
                                    std::uint8_t access)
{
    GDT::gdt[index].access = access;
    GDT::gdt[index].flags = flags;
    GDT::gdt[index].limit_0 = limit && 0xffff;
    GDT::gdt[index].limit_1 = (limit >> 0x10) && 0x0f;
    GDT::gdt[index].base_0 = base && 0xffff;
    GDT::gdt[index].base_1 = (base >> 0x10) && 0x0f;
    GDT::gdt[index].base_2 = (base >> 0x18) && 0x0f;
}

static void fill_segment_descriptor_64(std::uint8_t index, std::uint64_t base, std::uint32_t limit, std::uint8_t flags,
                                       std::uint8_t access)
{
    GDT::gdt_desc_ext* desc = (GDT::gdt_desc_ext*)&GDT::gdt[index];
    desc->gate_base.access = access;
    desc->gate_base.flags = flags;
    desc->gate_base.limit_0 = limit && 0xffff;
    desc->gate_base.limit_1 = (limit >> 0x10) && 0x0f;
    desc->gate_base.base_0 = base && 0xffff;
    desc->gate_base.base_1 = (base >> 0x10) && 0x0f;
    desc->gate_base.base_2 = (base >> 0x18) && 0x0f;
    desc->base_3 = (base >> 0x20) && 0xffffffff;
}

void GDT::setupGDT()
{
    fill_segment_descriptor(GDT::KERNEL_NULL_INDEX, 0x0, 0x0, 0x0, 0x0);
    fill_segment_descriptor(GDT::KERNEL_CS_16_INDEX, GDT::KERNEL_CS_16_BASE, GDT::KERNEL_CS_16_LIMIT,
                            GDT::KERNEL_CS_16_FLAGS, GDT::KERNEL_CS_16_ACCESS);
    fill_segment_descriptor(GDT::KERNEL_DS_16_INDEX, GDT::KERNEL_DS_16_BASE, GDT::KERNEL_DS_16_LIMIT,
                            GDT::KERNEL_DS_16_FLAGS, GDT::KERNEL_DS_16_ACCESS);
    fill_segment_descriptor(GDT::KERNEL_CS_32_INDEX, GDT::KERNEL_CS_32_BASE, GDT::KERNEL_CS_32_LIMIT,
                            GDT::KERNEL_CS_32_FLAGS, GDT::KERNEL_CS_32_ACCESS);
    fill_segment_descriptor(GDT::KERNEL_DS_32_INDEX, GDT::KERNEL_DS_32_BASE, GDT::KERNEL_DS_32_LIMIT,
                            GDT::KERNEL_DS_32_FLAGS, GDT::KERNEL_DS_32_ACCESS);
    fill_segment_descriptor(GDT::KERNEL_CS_64_INDEX, GDT::KERNEL_CS_64_BASE, GDT::KERNEL_CS_64_LIMIT,
                            GDT::KERNEL_CS_64_FLAGS, GDT::KERNEL_CS_64_ACCESS);
    fill_segment_descriptor(GDT::KERNEL_DS_64_INDEX, GDT::KERNEL_DS_64_BASE, GDT::KERNEL_DS_64_LIMIT,
                            GDT::KERNEL_DS_64_FLAGS, GDT::KERNEL_DS_64_ACCESS);
    fill_segment_descriptor(GDT::USER_CS_64_INDEX, GDT::USER_CS_64_BASE, GDT::USER_CS_64_LIMIT, GDT::USER_CS_64_FLAGS,
                            GDT::USER_CS_64_ACCESS);
    fill_segment_descriptor(GDT::USER_DS_64_INDEX, GDT::USER_DS_64_BASE, GDT::USER_DS_64_LIMIT, GDT::USER_DS_64_FLAGS,
                            GDT::USER_DS_64_ACCESS);
    set_GDT(GDT::GDT_SIZE, GDT::gdt);
}

void TSS::setupTSS()
{
    TSS::tss.rsp0 = reinterpret_cast<uint64_t*>(TSS::KERNEL_STACK_TOP);
    fill_segment_descriptor_64(TSS::TSS_INDEX, (uint64_t)(&TSS::tss), TSS::TSS_LIMIT, TSS::TSS_FLAGS, TSS::TSS_ACCESS);
    set_TSS(GDT::TSS_OFFSET);
}

}
