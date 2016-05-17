/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "SetupProcessor.hpp"
#include "multiboot2.h"
#include "processor_struct.hpp"

#include <io.h>

// Size Bits
#define SZ_A		0x1
#define SZ_L		0x2
#define SZ_D		0x4
#define SZ_G		0x8

// Common flags
#define FLAG_P 		0x80
#define FLAG_DPL0	0x00
#define FLAG_DPL1	0x20
#define FLAG_DPL2	0x40
#define FLAG_DPL3	0x60

// Code flags
#define FLAG_A		0x01
#define FLAG_R		0x02
#define FLAG_C		0x04
#define FLAG_CODE	0x18

// Data flags
#define FLAG_W		0x02
#define FLAG_E		0x04
#define FLAG_DATA	0x10

// System Descriptor Type Flags
#define FLAG_LDT		0x02
#define FLAG_TSS_AVL	0x09
#define FLAG_TSS_BSY	0x0B
#define FLAG_CALL		0x0C
#define FLAG_INT		0x0E
#define FLAG_TRAP		0x0F

// Selector index
#define SEL_NULL		0x00
#define SEL_KERNEL_CS	0x10
#define SEL_KERNEL_DS	0x18
#define SEL_TSS			0x20
#define SEL_USER_CS		0x43
#define SEL_USER_DS		0x4A
#define SEL_USER_CS_32	0x53
#define SEL_USER_DS_32	0x5A

// TSS Infos
#define TSS_INDEX		(SEL_TSS >> 3)
#define TSS_BASE		((uint64_t)&tss)
#define TSS_LIMIT		sizeof(tss)
#define TSS_FLAGS		(FLAG_DPL0 | FLAG_TSS_AVL)
#define TSS_SIZEB		0

// USER_CS Infos
#define USER_CS_INDEX	(SEL_USER_CS >> 3)
#define USER_CS_BASE	0x00				// ignored
#define USER_CS_LIMIT	0x00				// ignored
#define USER_CS_FLAGS	FLAG_DPL3
#define USER_CS_SIZEB	SZ_L

// USER_DS Infos
#define USER_DS_INDEX	(SEL_USER_DS >> 3)
#define USER_DS_BASE	0x00				// ignored
#define USER_DS_LIMIT	0x00				// ignored
#define USER_DS_FLAGS	(FLAG_DPL3 | FLAG_DATA | FLAG_W)
#define USER_DS_SIZEB	0x00				// ignored

// USER_CS_32 Infos
#define USER_CS_32_INDEX	(SEL_USER_CS_32 >> 3)
#define USER_CS_32_BASE		0x00
#define USER_CS_32_LIMIT	(uint32_t)(-1)
#define USER_CS_32_FLAGS	(FLAG_DPL3 | FLAG_CODE)
#define USER_CS_32_SIZEB	SZ_D

// USER_DS_32 Infos
#define USER_DS_32_INDEX	(SEL_USER_DS_32 >> 3)
#define USER_DS_32_BASE		0x00
#define USER_DS_32_LIMIT	(uint32_t)(-1)
#define USER_DS_32_FLAGS	(FLAG_DPL3 | FLAG_DATA | FLAG_W)
#define USER_DS_32_SIZEB	SZ_D

static void fill_segment_descriptor_64(uint8_t index, uint64_t base, uint32_t limit,
		uint8_t flags, uint8_t sizebits)
{
	uint32_t p0, p1, p2, p3;

	if (limit > 0xFFFFF) {
		limit >>= 12;
		sizebits |= SZ_G;
	}

	p0 = (limit & 0xFFFF) + ((base & 0xFFFF) << 16);
	p1 = (base >> 16) & 0xFF;
	p1 |= ((flags | FLAG_P) & 0xEF) << 8;
	p1 |= limit & 0xF0000;
	p1 |= (sizebits & 0xF) << 20;
	p1 |= base & 0xFF000000;
	p2 = base >> 32;
	p3 = 0;

	gdt[index] = (((uint64_t) p1) << 32) | p0;
	gdt[index+1] = (((uint64_t) p3) << 32) | p2;
}

static void fill_segment_descriptor(uint8_t index, uint32_t base, uint32_t limit,
		uint8_t flags, uint8_t sizebits)
{
	uint32_t p0, p1;

	if (limit > 0xFFFFF) {
		limit >>= 12;
		sizebits |= SZ_G;
	}

	p0 = (limit & 0xFFFF) + ((base & 0xFFFF) << 16);
	p1 = (base >> 16) & 0xFF;
	p1 |= ((flags | FLAG_P) & 0xFF) << 8;
	p1 |= limit & 0xF0000;
	p1 |= (sizebits & 0xF) << 20;
	p1 |= base & 0xFF000000;

	gdt[index] = (((uint64_t) p1) << 32) | p0;

}

static void fill_idt_descriptor_64(uint8_t index, uint64_t offset, uint16_t selector,
		uint8_t flags, uint8_t ist)
{
	struct gate_desc* gate = &(idt[index]);

	gate->offset_1 = offset & 0xFFFF;
	gate->selector = selector;
	gate->ist = ist;
	gate->reserved_1 = 0;
	gate->flags = (flags | FLAG_P) & 0xEF;
	gate->offset_2 = (offset >> 16) & 0xFFFF;
	gate->offset_3 = (offset >> 32);
	gate->reserved_2 = 0;
}

void SetupProcessor::setupGDT()
{
	// KERNEL_CS and KERNEL_DS are already set
	// TSS is set separately
	fill_segment_descriptor(USER_CS_INDEX, USER_CS_BASE,
			USER_CS_LIMIT, USER_CS_FLAGS, USER_CS_SIZEB);
	fill_segment_descriptor(USER_DS_INDEX, USER_DS_BASE,
			USER_DS_LIMIT, USER_DS_FLAGS, USER_DS_SIZEB);
	fill_segment_descriptor(USER_CS_32_INDEX, USER_CS_32_BASE,
			USER_CS_32_LIMIT, USER_CS_FLAGS, USER_CS_32_SIZEB);
	fill_segment_descriptor(USER_DS_32_INDEX, USER_DS_32_BASE,
			USER_DS_32_LIMIT, USER_DS_32_FLAGS, USER_DS_32_SIZEB);
}


void SetupProcessor::setupIDT()
{
	uint8_t idt_flags = FLAG_P | FLAG_DPL0 | FLAG_INT;
	fill_idt_descriptor_64(49, (uint64_t)default_handler, SEL_KERNEL_CS, idt_flags, 1);
	set_IDT(sizeof(idt)-1, idt);
}

void SetupProcessor::setupTSS()
{
	tss.rsp0 = &stack_top;
	tss.ist1 = &stack_top_ist1;
	fill_segment_descriptor_64(TSS_INDEX, TSS_BASE,
			TSS_LIMIT, TSS_FLAGS, TSS_SIZEB);
	set_TSS(SEL_TSS);
}

void SetupProcessor::copyMultibootInfo()
{
	uint32_t* orig_multiboot_info_ptr = (uint32_t*)(uint64_t)multiboot_info;
	uint32_t size = ((orig_multiboot_info_ptr[0]-1)>> 2) + 1;
	for (uint32_t i = 0; i < size ; ++i) {
		multiboot_info_tags[i] = orig_multiboot_info_ptr[i];
	}
}

void SetupProcessor::setupPIC()
{
	/* Initialize the master. */
	outb(0x20, 0x11);		// Init command
	outb(0x21, 0x20);		// Set offset (IRQ0->7 use IVT[0x20->0x27])
	outb(0x21, 0x4);		// Set slave at IRQ2
	outb(0x21, 0x1);		// Set 8086 mode ?

	/* Initialize the slave. */
	outb(0xa0, 0x11);		// Init command
	outb(0xa1, 0x28);		// Set offset (IRQ8->15 use IVT[0x28->0x2F]
	outb(0xa1, 0x2);		// Set master
	outb(0xa1, 0x1);		// Set 8086 mode ?

	/* Disable all IRQs */
	outb(0x21, 0xff);
	outb(0xa1, 0xff);

	/* Ack any bogus intrs by setting the End Of Interrupt bit. */
	outb(0x20, 0x20);
	outb(0xa0, 0x20);
}

void SetupProcessor::setupMemoryMapping() {
	// Nothing yet
}

void SetupProcessor::setupAll()
{
	copyMultibootInfo();
	setupGDT();
	setupIDT();
	setupTSS();
	setupPIC();
}
