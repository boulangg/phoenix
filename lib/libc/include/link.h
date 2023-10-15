/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include <elf64.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct dl_phdr_info
{
    Elf64_Addr dlpi_addr;        /* Base address of object */
    const char* dlpi_name;       /* (Null-terminated) name of object */
    const Elf64_Phdr* dlpi_phdr; /* Pointer to array of ELF program headers for this object */
    Elf64_Half dlpi_phnum;       /* # of items in dlpi_phdr */
} dl_phdr_info;

int dl_iterate_phdr(int (*callback)(struct dl_phdr_info* info, size_t size, void* data), void* data);

typedef struct link_map
{
    Elf64_Addr l_addr;       /* Difference between the address in the ELF file and the addresses in memory.  */
    char* l_name;            /* Absolute file name object was found in.  */
    Elf64_Dyn* l_ld;         /* Dynamic section of the shared object.  */
    struct link_map* l_next; /* Chain of loaded objects.  */
    struct link_map* l_prev;
} link_map;

#ifdef __cplusplus
}
#endif
