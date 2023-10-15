/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef _BOOT_PROCESSOR_STRUCT_
#define _BOOT_PROCESSOR_STRUCT_

#define GDT_ENTRIES 512
#define IDT_ENTRIES 256

#define GDT_SIZE (GDT_ENTRIES*8-1)
#define IDT_SIZE (IDT_ENTRIES*16-1)

#define MULTIBOOT_INFO_SIZE 8192

#ifndef ASM_FILE

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

	void default_handler();
	void reserved_handler();

	struct gate_desc
	{
		uint64_t offset_1 : 16;
		uint16_t selector : 16;
		uint8_t ist : 3;
		uint64_t reserved_1 : 5;
		uint8_t flags : 8;
		uint64_t offset_2 : 16;
		uint64_t offset_3 : 32;
		uint64_t reserved_2 : 32;
	} __attribute__((packed));

	struct page_entry
	{
		uint8_t P : 1;
		uint8_t RW : 1;
		uint8_t US : 1;
		uint8_t PWT : 1;
		uint8_t PCD : 1;
		uint8_t A : 1;
		uint8_t IGN : 1;
		uint8_t MBZ : 2;
		uint8_t AVL : 3;
		uint64_t base_addr : 40;
		uint16_t Available : 11;
		uint8_t NX : 1;
	} __attribute__((packed));

	struct x86_64_tss
	{
		uint32_t reserved_1 : 32;
		uint64_t* rsp0;				/* stack pointer for ring 0 */
		uint64_t* rsp1;				/* stack pointer for ring 1 */
		uint64_t* rsp2;				/* stack pointer for ring 2 */
		uint64_t reserved_2;
		uint64_t* ist1;				/* interrupt stack pointer 1 */
		uint64_t* ist2;				/* interrupt stack pointer 2 */
		uint64_t* ist3;				/* interrupt stack pointer 3 */
		uint64_t* ist4;				/* interrupt stack pointer 4 */
		uint64_t* ist5;				/* interrupt stack pointer 5 */
		uint64_t* ist6;				/* interrupt stack pointer 6 */
		uint64_t* ist7;				/* interrupt stack pointer 7 */
		uint64_t reserved_3;
		uint16_t reserved_4;
		uint16_t io_bit_map_offset;	/* offset to start of IO permission bit map */
	} __attribute__((packed));

	struct apps_desc
	{
		char(*apps_name)[];
		uint64_t* apps_start;
		uint64_t* apps_end;
	};

	struct apps_symbol_table
	{
		uint32_t nb_user_apps;
		struct apps_desc apps[];
	};

	extern struct apps_symbol_table user_apps_symbol_table;

	extern struct gate_desc idt[IDT_ENTRIES];
	extern uint64_t kernel_pml4t[512];
	extern uint32_t kernel_page_limit;
	extern uint64_t _kernel_start;
	extern uint64_t _kernel_end;
	extern uint64_t _bootstrap_stack_bottom;
	extern uint64_t _bootstrap_stack_top;

	extern uint64_t gdt[];
	extern struct x86_64_tss tss;
	extern uint64_t stack_top;
	extern uint64_t stack_top_ist1;
	extern uint32_t multiboot_info_tags[MULTIBOOT_INFO_SIZE];
	extern uint32_t multiboot_magic;
	extern uint32_t multiboot_info;

#ifdef __cplusplus
}
#endif

#endif

#endif // _BOOT_PROCESSOR_STRUCT_
