/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "SetupProcessor.hpp"

#include <stdio.h>

#include <include/constant.h>
#include <asm/io.h>
#include <core/Console.hpp>
#include <mm/PhysicalAllocator.hpp>
#include <mm/Page.hpp>
#include <mm/PageTable.hpp>

#include "multiboot2.h"

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
	set_GDT(GDT_SIZE, gdt);
}


void SetupProcessor::setupIDT()
{
	uint8_t idt_flags = FLAG_P | FLAG_DPL0 | FLAG_INT;
	fill_idt_descriptor_64(49, (uint64_t)default_handler, SEL_KERNEL_CS, idt_flags, 1);
	set_IDT(sizeof(idt)-1, idt);
}

void SetupProcessor::setupTSS()
{
	tss.rsp0 = (uint64_t*)KERNEL_STACK_TOP;
	tss.ist1 = (uint64_t*)KERNEL_IST1_TOP;
	fill_segment_descriptor_64(TSS_INDEX, TSS_BASE,
			TSS_LIMIT, TSS_FLAGS, TSS_SIZEB);
	set_TSS(SEL_TSS);
}

void SetupProcessor::copyMultibootInfo()
{
	uint32_t* orig_multiboot_info_ptr = (uint32_t*)(uint64_t)multiboot_info;
	uint32_t size = ((orig_multiboot_info_ptr[0]-1)>> 2) + 1;
	if (size > MULTIBOOT_INFO_SIZE) {
		size = MULTIBOOT_INFO_SIZE;
	}
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

static uint64_t* get_free_page(uint64_t nb_pages, uint64_t page_size) {
	uint64_t* physical_pages = (uint64_t*)(uint64_t)kernel_page_limit;
	kernel_page_limit += nb_pages*page_size;
	for (uint64_t i = 0; i < nb_pages*page_size/8; i++) {
		physical_pages[i] = 0;
	}
	return physical_pages;
}

static void initPageArray(Page* page_array, uint64_t nb_phys_pages, multiboot_tag_mmap *mmap) {
	for (uint64_t i=0; i < nb_phys_pages; ++i) {
		page_array[i].physAddr = (uint64_t*)(i*PAGE_SIZE);
		page_array[i].kernelMappAddr = (uint64_t*)(i*PAGE_SIZE+KERNEL_START);
		page_array[i].type = PageType::UNUSABLE;
	}

	multiboot_mmap_entry* mmap_entry;
	mmap_entry = mmap->entries;
	while ((char*)mmap_entry < ((char*)mmap + mmap->size)) {
		if (mmap_entry->type == MULTIBOOT_MEMORY_AVAILABLE) {
			uint64_t index_start = (mmap_entry->addr + PAGE_SIZE - 1) / PAGE_SIZE;
			uint64_t index_end = (mmap_entry->addr + mmap_entry->len) / PAGE_SIZE;

			for (uint64_t i = index_start; i < index_end; i++) {
				page_array[i].type = PageType::FREE;
			}
		}
		mmap_entry = (multiboot_memory_map_t *)((unsigned long)mmap_entry + mmap->entry_size);
	}

	uint64_t kernel_index_start = (((uint64_t)&_kernel_start)- KERNEL_HIGH_VMA) / PAGE_SIZE;
	uint64_t kernel_index_end = (kernel_page_limit) / PAGE_SIZE;

	for (uint64_t i = kernel_index_start; i < kernel_index_end; i++) {
		page_array[i].type = PageType::KERNEL;
	}



}

static void mapLinearPage(uint64_t* PML4T, uint64_t* physAddr, uint64_t* virtAddr) {
	uint16_t highLvlFlags = 0x3;
	uint16_t flags = 0x3;
	uint64_t PML4T_index = ((uint64_t)virtAddr >> PML4E_INDEX_OFFSET) & PML4E_INDEX_MASK;
	if (PML4T[PML4T_index] == 0) {
		PML4T[PML4T_index] = (uint64_t)get_free_page(1, PAGE_SIZE) | highLvlFlags;
	}
	uint64_t* PDPT = (uint64_t*)(PML4T[PML4T_index] & PAGE_ADDR_MASK);
	uint64_t PDPT_index = ((uint64_t)virtAddr >> PDPE_INDEX_OFFSET) & PDPE_INDEX_MASK;
	if (PDPT[PDPT_index] == 0) {
		PDPT[PDPT_index] = (uint64_t)get_free_page(1, PAGE_SIZE) | highLvlFlags;
	}
	uint64_t* PD = (uint64_t*)(PDPT[PDPT_index] & PAGE_ADDR_MASK);
	uint64_t PD_index = ((uint64_t)virtAddr >> PDE_INDEX_OFFSET) & PDE_INDEX_MASK;
	if (PD[PD_index] == 0) {
		PD[PD_index] = (uint64_t)get_free_page(1, PAGE_SIZE) | highLvlFlags;
	}
	uint64_t* PT = (uint64_t*)(PD[PD_index] & PAGE_ADDR_MASK);
	uint64_t PTE_index = ((uint64_t)virtAddr >> PTE_INDEX_OFFSET) & PTE_INDEX_MASK;

	uint64_t PTE = (uint64_t)physAddr & PAGE_ADDR_MASK;
	PT[PTE_index] = PTE | flags;
}

void SetupProcessor::setupMemoryMapping() {
	uint64_t max_available_addr = 0x0;
	multiboot_tag *tag;
	tag = (multiboot_tag*) (multiboot_info_tags+2);
	while (tag->type != MULTIBOOT_TAG_TYPE_MMAP &&
			tag->type != MULTIBOOT_TAG_TYPE_END) {
		tag=(multiboot_tag *)((uint8_t*)tag+((tag->size+7)&~7));
	}

	if (tag->type == MULTIBOOT_TAG_TYPE_MMAP) {
		multiboot_tag_mmap *mmap = (multiboot_tag_mmap*)tag;
		multiboot_mmap_entry* mmap_entry;
		mmap_entry = mmap->entries;
		while ((char*)mmap_entry < ((char*)tag + tag->size)) {
			uint64_t mmap_entry_addr_end = mmap_entry->addr + mmap_entry->len;
			if (mmap_entry->type == MULTIBOOT_MEMORY_AVAILABLE) {
				if ( mmap_entry_addr_end > max_available_addr) {
					max_available_addr = mmap_entry_addr_end;
				}
			}
			// Finish the full linear memory mapping

			uint64_t mmap_entry_addr_start = mmap_entry->addr;
			if (mmap_entry_addr_start < EARLY_FULL_MAPPING) {
				mmap_entry_addr_start = EARLY_FULL_MAPPING;
			}
			for (uint64_t i = mmap_entry_addr_start; i < mmap_entry_addr_end; i+= PAGE_SIZE) {
				mapLinearPage(kernel_pml4t, (uint64_t*)i, (uint64_t*)(KERNEL_MAPPING_START+i));
			}

			mmap_entry = (multiboot_memory_map_t *)((unsigned long)mmap_entry + mmap->entry_size);
		}

		// Initialize physical pages allocator
		uint64_t nb_physical_pages = (max_available_addr + PAGE_SIZE-1) / PAGE_SIZE;
		uint64_t nb_pages_required = (nb_physical_pages*sizeof(Page) + PAGE_SIZE-1) / PAGE_SIZE;
		Page* page_array = (Page*)get_free_page(nb_pages_required, PAGE_SIZE);
		page_array = (Page*)((uint64_t)page_array+KERNEL_MAPPING_START);
		initPageArray(page_array, nb_physical_pages, mmap);
		PhysicalAllocator::initAllocator(page_array, nb_physical_pages);

		// Setup interruption stack
		PageTable kernelPageTable = PageTable::getKernelPageTable();
		for (uint64_t addr = KERNEL_IST1_BOTTOM; addr < KERNEL_IST1_TOP; addr+=PAGE_SIZE)  {
			Page* page = PhysicalAllocator::allocZeroedPage();
			kernelPageTable.mapPage(page->physAddr, (uint64_t*)addr, 0x3, 0x3);
		}

		// Clear low memory mapping
		kernel_pml4t[0] = 0;
	}
}

void SetupProcessor::setupAll()
{
	(apps_symbol_table)user_apps_symbol_table;
	copyMultibootInfo();
	setupGDT();
	setupIDT();
	setupTSS();
	setupPIC();
	setupMemoryMapping();
}
