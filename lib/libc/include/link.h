#pragma once

#include <elf64.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct dl_phdr_info
	{
		Elf64_Addr        	dlpi_addr; 		/* Base address of object */
		const char* dlpi_name; 	/* (Null-terminated) name of object */
		const Elf64_Phdr* dlpi_phdr; 	/* Pointer to array of ELF program headers for this object */
		Elf64_Half       	dlpi_phnum; 	/* # of items in dlpi_phdr */
	} dl_phdr_info;

	int dl_iterate_phdr(int (*callback)(struct dl_phdr_info* info, size_t size, void* data), void* data);

#ifdef __cplusplus
}
#endif
