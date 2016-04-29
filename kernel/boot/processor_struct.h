/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef KERNEL_BOOT_PROCESS_STRUCT_
#define KERNEL_BOOT_PROCESS_STRUCT_

#define GDT_ENTRIES 512
#define IDT_ENTRIES 256

#ifndef ASM_FILE

#include <stddef.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

void default_handler();
void reserved_handler();

struct gate_desc {
	uint64_t offset_1:16;
	uint16_t selector:16;
	uint8_t ist:3;
	uint64_t reserved_1:5;
	uint8_t flags:8;
	uint64_t offset_2:16;
	uint64_t offset_3:32;
	uint64_t reserved_2:32;
}__attribute__ ((packed));

struct x86_64_tss {
	uint32_t reserved_1:32;
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
} __attribute__ ((packed));

extern struct gate_desc idt[IDT_ENTRIES];
extern uint64_t gdt[];
extern struct x86_64_tss tss;
extern uint64_t stack_top;
extern uint64_t stack_top_ist1;

void setup_gdt();
void setup_idt();
void setup_tss();
void setup_cpu();

extern void setIDT(uint16_t limit, struct gate_desc* base);
extern void setGDT(uint16_t limit, uint64_t* base);
extern void setTSS(uint16_t selector);


#ifdef __cplusplus
}
#endif

#endif

#endif // KERNEL_BOOT_PROCESS_STRUCT_
