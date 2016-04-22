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

extern uint64_t gdt[];

#ifdef __cplusplus
}
#endif

#endif

#endif // KERNEL_BOOT_PROCESS_STRUCT_
