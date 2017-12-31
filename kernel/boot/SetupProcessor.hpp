/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef _BOOT_SETUP_PROCESSOR_
#define _BOOT_SETUP_PROCESSOR_

#include "processor_struct.hpp"

#ifndef ASM_FILE

#ifdef __cplusplus
extern "C" {
#endif

extern void set_IDT(uint16_t limit, struct gate_desc* base);
extern void set_GDT(uint16_t limit, uint64_t* base);
extern void set_TSS(uint16_t selector);
extern void set_CR3(uint64_t pml4t);
extern uint64_t get_CR3();
extern void enable_syscall();
extern void load_syscall(uint64_t STAR,  uint64_t LSTAR, uint64_t CSTAR, uint32_t SFMASK);
extern void syscall64_handler();

extern void default_handler();
extern void default_handler_error_code();
extern void reserved_handler();
extern void EXC_14_handler();

uint64_t syscall64(uint64_t a, uint64_t b, uint64_t c, uint64_t d, uint64_t e, uint64_t f, uint64_t num);

extern void IT_32_handler();
extern void IT_33_handler();

#ifdef __cplusplus
}
#endif

class SetupProcessor {
public:
	static void setupAll();

private:
	static void copyMultibootInfo();
	static void setupGDT();
	static void setupIDT();
	static void setupTSS();
	static void setupPIC();
	static void setupMemoryMapping();
	static void setupSyscall();
	static void setupGlobalConstructors();
};

#endif // ASM_FILE

#endif //_BOOT_SETUP_PROCESSOR_

