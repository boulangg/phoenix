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

extern void default_handler();
extern void reserved_handler();

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
};

#endif // ASM_FILE

#endif //_BOOT_SETUP_PROCESSOR_

