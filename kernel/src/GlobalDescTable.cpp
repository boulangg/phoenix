/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at
 * https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "GlobalDescTable.h"

#include "Constant.h"

namespace kernel {

static void fill_segment_descriptor(GDT::gdt_desc gdt[], std::uint8_t index, std::uint64_t base, std::uint32_t limit,
                                    std::uint8_t flags, std::uint8_t access)
{
    gdt[index].limit_0 = limit & 0xFFFF;
    gdt[index].base_0 = base & 0xFFFF;
    gdt[index].base_1 = (base >> 0x10) & 0xFF;
    gdt[index].access = access & 0xFF;
    gdt[index].limit_1 = (limit >> 0x10) & 0x0F;
    gdt[index].flags = flags & 0x0F;
    gdt[index].base_2 = (base >> 0x18) & 0xFF;
}

static void fill_segment_descriptor_64(GDT::gdt_desc gdt[], std::uint8_t index, std::uint64_t base, std::uint32_t limit,
                                       std::uint8_t flags, std::uint8_t access)
{
    GDT::gdt_desc_ext* desc = (GDT::gdt_desc_ext*)&gdt[index];
    desc->gate_base.limit_0 = limit & 0xFFFF;
    desc->gate_base.base_0 = base & 0xFFFF;
    desc->gate_base.base_1 = (base >> 0x10) & 0xFF;
    desc->gate_base.access = access & 0xFF;
    desc->gate_base.limit_1 = (limit >> 0x10) & 0x0F;
    desc->gate_base.flags = flags & 0x0F;
    desc->gate_base.base_2 = (base >> 0x18) & 0xFF;
    desc->base_3 = (base >> 0x20) & 0xFFFFFFFF;
}

namespace GDT {

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

static constexpr std::uint8_t KERNEL_NULL_INDEX = KERNEL_NULL_OFFSET >> 3;

static constexpr std::uint16_t KERNEL_CS_16_INDEX = KERNEL_CS_16_OFFSET >> 3;
static constexpr std::uint16_t KERNEL_CS_16_BASE = 0x00;
static constexpr std::uint16_t KERNEL_CS_16_LIMIT = 0xFFFF;
static constexpr std::uint8_t KERNEL_CS_16_FLAGS = Flags::FLAG_SIZE_16;
static constexpr std::uint8_t KERNEL_CS_16_ACCESS =
    Access::ACCESS_PA | Access::ACCESS_DPL0 | Access::ACCESS_TYPE_CODE | Access::Code::ACCESS_READ_ALLOWED;

static constexpr std::uint16_t KERNEL_DS_16_INDEX = KERNEL_DS_16_OFFSET >> 3;
static constexpr std::uint16_t KERNEL_DS_16_BASE = 0x00;
static constexpr std::uint16_t KERNEL_DS_16_LIMIT = 0xFFFF;
static constexpr std::uint8_t KERNEL_DS_16_FLAGS = Flags::FLAG_SIZE_16;
static constexpr std::uint8_t KERNEL_DS_16_ACCESS =
    Access::ACCESS_PA | Access::ACCESS_DPL0 | Access::ACCESS_TYPE_DATA | Access::Data::ACCESS_WRITE_ALLOWED;

static constexpr std::uint16_t KERNEL_CS_32_INDEX = KERNEL_CS_32_OFFSET >> 3;
static constexpr std::uint32_t KERNEL_CS_32_BASE = 0x00;
static constexpr std::uint32_t KERNEL_CS_32_LIMIT = 0xFFFFFFFF;
static constexpr std::uint8_t KERNEL_CS_32_FLAGS = Flags::FLAG_SIZE_32;
static constexpr std::uint8_t KERNEL_CS_32_ACCESS =
    Access::ACCESS_PA | Access::ACCESS_DPL0 | Access::ACCESS_TYPE_CODE | Access::Code::ACCESS_READ_ALLOWED;

static constexpr std::uint32_t KERNEL_DS_32_INDEX = KERNEL_DS_32_OFFSET >> 3;
static constexpr std::uint32_t KERNEL_DS_32_BASE = 0x00;
static constexpr std::uint32_t KERNEL_DS_32_LIMIT = 0xFFFFFFFF;
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

gdt_desc gdt[GDT_ENTRIES] __attribute__((aligned(4096)));

void setupGDT()
{
    fill_segment_descriptor(gdt, KERNEL_NULL_INDEX, 0x0, 0x0, 0x0, 0x0);
    fill_segment_descriptor(gdt, KERNEL_CS_16_INDEX, KERNEL_CS_16_BASE, KERNEL_CS_16_LIMIT, KERNEL_CS_16_FLAGS,
                            KERNEL_CS_16_ACCESS);
    fill_segment_descriptor(gdt, KERNEL_DS_16_INDEX, KERNEL_DS_16_BASE, KERNEL_DS_16_LIMIT, KERNEL_DS_16_FLAGS,
                            KERNEL_DS_16_ACCESS);
    fill_segment_descriptor(gdt, KERNEL_CS_32_INDEX, KERNEL_CS_32_BASE, KERNEL_CS_32_LIMIT, KERNEL_CS_32_FLAGS,
                            KERNEL_CS_32_ACCESS);
    fill_segment_descriptor(gdt, KERNEL_DS_32_INDEX, KERNEL_DS_32_BASE, KERNEL_DS_32_LIMIT, KERNEL_DS_32_FLAGS,
                            KERNEL_DS_32_ACCESS);
    fill_segment_descriptor(gdt, KERNEL_CS_64_INDEX, KERNEL_CS_64_BASE, KERNEL_CS_64_LIMIT, KERNEL_CS_64_FLAGS,
                            KERNEL_CS_64_ACCESS);
    fill_segment_descriptor(gdt, KERNEL_DS_64_INDEX, KERNEL_DS_64_BASE, KERNEL_DS_64_LIMIT, KERNEL_DS_64_FLAGS,
                            KERNEL_DS_64_ACCESS);
    fill_segment_descriptor(gdt, USER_CS_64_INDEX, USER_CS_64_BASE, USER_CS_64_LIMIT, USER_CS_64_FLAGS,
                            USER_CS_64_ACCESS);
    fill_segment_descriptor(gdt, USER_DS_64_INDEX, USER_DS_64_BASE, USER_DS_64_LIMIT, USER_DS_64_FLAGS,
                            USER_DS_64_ACCESS);
    set_GDT(GDT_SIZE, gdt);
}

}

namespace TSS {

static constexpr std::uint64_t TSS_INDEX = GDT::TSS_OFFSET >> 3;
static constexpr std::uint32_t TSS_LIMIT = sizeof(tss);
static constexpr std::uint8_t TSS_FLAGS = 0;
static constexpr std::uint8_t TSS_ACCESS = (GDT::Access::ACCESS_P | GDT::Access::ACCESS_DPL0 |
                                            GDT::Access::ACCESS_TYPE_SYS | GDT::Access::System::ACCESS_TYPE_64_AVAIL);

tss_desc tss;

void setupTSS()
{
    tss.rsp0 = reinterpret_cast<uint64_t*>(KERNEL_STACK_TOP);
    fill_segment_descriptor_64(GDT::gdt, TSS_INDEX, (uint64_t)(&tss), TSS_LIMIT, TSS_FLAGS, TSS_ACCESS);
    set_TSS(GDT::TSS_OFFSET);
}
}

}
