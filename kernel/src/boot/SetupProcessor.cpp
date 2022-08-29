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
#include <core/Clock.hpp>
#include <proc/ProcessScheduler.hpp>

#include <core/interrupt/InterruptManager.hpp>




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
	p1 |= ((flags | FLAG_P) & 0xFF) << 8;
	p1 |= limit & 0xF0000;
	p1 |= (sizebits & 0xF) << 20;
	p1 |= base & 0xFF000000;
	p2 = base >> 32;
	p3 = 0;

	gdt[index] = (((uint64_t)p1) << 32) | p0;
	gdt[index + 1] = (((uint64_t)p3) << 32) | p2;
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

	gdt[index] = (((uint64_t)p1) << 32) | p0;

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
	set_IDT(sizeof(idt) - 1, idt);
}

void SetupProcessor::setupTSS()
{
	tss.rsp0 = (uint64_t*)KERNEL_STACK_TOP;
	tss.ist1 = (uint64_t*)USER_INT_STACK_END;
	fill_segment_descriptor_64(TSS_INDEX, TSS_BASE,
							   TSS_LIMIT, TSS_FLAGS, TSS_SIZEB);
	set_TSS(SEL_TSS);
}

void SetupProcessor::copyMultibootInfo()
{
	uint32_t* orig_multiboot_info_ptr = (uint32_t*)(uint64_t)multiboot_info;
	uint32_t size = ((orig_multiboot_info_ptr[0] - 1) >> 2) + 1;
	if (size > MULTIBOOT_INFO_SIZE) {
		size = MULTIBOOT_INFO_SIZE;
	}
	for (uint32_t i = 0; i < size; ++i) {
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

static uint64_t* get_free_page(uint64_t nb_pages, uint64_t page_size)
{
	uint64_t* physical_pages = (uint64_t*)(uint64_t)kernel_page_limit;
	kernel_page_limit += nb_pages * page_size;
	for (uint64_t i = 0; i < nb_pages * page_size / 8; i++) {
		physical_pages[i] = 0;
	}
	return physical_pages;
}

static void initPageArray(Page* page_array, uint64_t nb_phys_pages, multiboot_tag_mmap* mmap)
{
	for (uint64_t i = 0; i < nb_phys_pages; ++i) {
		page_array[i].physAddr = (uint64_t*)(i * PAGE_SIZE);
		page_array[i].kernelMappAddr = (uint64_t*)(i * PAGE_SIZE + KERNEL_START);
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
		mmap_entry = (multiboot_memory_map_t*)((unsigned long)mmap_entry + mmap->entry_size);
	}

	uint64_t kernel_index_start = (((uint64_t)&_kernel_start) - KERNEL_HIGH_VMA) / PAGE_SIZE;
	uint64_t kernel_index_end = (kernel_page_limit) / PAGE_SIZE;

	for (uint64_t i = kernel_index_start; i < kernel_index_end; i++) {
		page_array[i].type = PageType::KERNEL;
	}



}

static void mapLinearPage(uint64_t* PML4T, uint64_t* physAddr, uint64_t* virtAddr)
{
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

void SetupProcessor::setupMemoryMapping()
{
	uint64_t max_available_addr = 0x0;
	multiboot_tag* tag;
	tag = (multiboot_tag*)(multiboot_info_tags + 2);
	while (tag->type != MULTIBOOT_TAG_TYPE_MMAP &&
		   tag->type != MULTIBOOT_TAG_TYPE_END) {
		tag = (multiboot_tag*)((uint8_t*)tag + ((tag->size + 7) & ~7));
	}

	if (tag->type == MULTIBOOT_TAG_TYPE_MMAP) {
		multiboot_tag_mmap* mmap = (multiboot_tag_mmap*)tag;
		multiboot_mmap_entry* mmap_entry;
		mmap_entry = mmap->entries;
		while ((char*)mmap_entry < ((char*)tag + tag->size)) {
			uint64_t mmap_entry_addr_end = mmap_entry->addr + mmap_entry->len;
			if (mmap_entry->type == MULTIBOOT_MEMORY_AVAILABLE) {
				if (mmap_entry_addr_end > max_available_addr) {
					max_available_addr = mmap_entry_addr_end;
				}
			}
			// Finish the full linear memory mapping

			uint64_t mmap_entry_addr_start = mmap_entry->addr;
			if (mmap_entry_addr_start < EARLY_FULL_MAPPING) {
				mmap_entry_addr_start = EARLY_FULL_MAPPING;
			}
			for (uint64_t i = mmap_entry_addr_start; i < mmap_entry_addr_end; i += PAGE_SIZE) {
				mapLinearPage(kernel_pml4t, (uint64_t*)i, (uint64_t*)(KERNEL_MAPPING_START + i));
			}

			mmap_entry = (multiboot_memory_map_t*)((unsigned long)mmap_entry + mmap->entry_size);
		}

		// Initialize physical pages allocator
		uint64_t nb_physical_pages = (max_available_addr + PAGE_SIZE - 1) / PAGE_SIZE;
		uint64_t nb_pages_required = (nb_physical_pages * sizeof(Page) + PAGE_SIZE - 1) / PAGE_SIZE;
		Page* page_array = (Page*)get_free_page(nb_pages_required, PAGE_SIZE);
		page_array = (Page*)((uint64_t)page_array + KERNEL_MAPPING_START);
		initPageArray(page_array, nb_physical_pages, mmap);
		PhysicalAllocator::initAllocator(page_array, nb_physical_pages);

		PageTable kernelPageTable = PageTable::getKernelPageTable();
		// Setup interruption stack
		for (uint64_t addr = KERNEL_IST1_BOTTOM; addr < KERNEL_IST1_TOP; addr += PAGE_SIZE) {
			Page* page = PhysicalAllocator::allocZeroedPage();
			kernelPageTable.mapPage(page->physAddr, (uint64_t*)addr, 0x3, 0x3);
		}

		// Setup syscall stack
		for (uint64_t addr = KERNEL_SYSCALL_BOTTOM; addr < KERNEL_SYSCALL_TOP; addr += PAGE_SIZE) {
			Page* page = PhysicalAllocator::allocZeroedPage();
			kernelPageTable.mapPage(page->physAddr, (uint64_t*)addr, 0x3, 0x3);
		}

		// Clear low memory mapping
		kernel_pml4t[0] = 0;
	}
}

void SetupProcessor::setupSyscall()
{
	enable_syscall();
	uint64_t STAR = ((((uint64_t)SEL_USER_CS_32) << 48) | (((uint64_t)SEL_KERNEL_CS) << 32) | 0);
	load_syscall(STAR, (uint64_t)syscall64_handler, 0, 0);
}

typedef void (*func_ptr)(void);
extern func_ptr _init_array_start[0], _init_array_end[0];

void SetupProcessor::setupGlobalConstructors()
{
	for (auto fn = _init_array_start; fn != _init_array_end; fn++) {
		(*fn)();
	}
}

void SetupProcessor::setupAll()
{
	(apps_symbol_table)user_apps_symbol_table;
	copyMultibootInfo();
	setupGDT();
	InterruptManager::setupIDT();
	setupTSS();
	InterruptManager::setupPIC();
	setupMemoryMapping();
	InterruptManager::setupSyscall();
	InterruptManager::setupHandlers();
	setupGlobalConstructors();
}
