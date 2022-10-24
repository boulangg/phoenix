#include "dl_helpers.h"

typedef struct dl_phdr_info
{
	Elf64_Addr dlpi_addr; 			/* Base address of object */
	const char* dlpi_name; 			/* (Null-terminated) name of object */
	const Elf64_Phdr* dlpi_phdr; 	/* Pointer to array of ELF program headers for this object */
	Elf64_Half dlpi_phnum;			/* # of items in dlpi_phdr */
} dl_phdr_info;

int dl_iterate_phdr(int (*callback)(struct dl_phdr_info* info, size_t size, void* data), void* data)
{
	dl_phdr_info info;
	for (struct link_map* l = &__main_link_map; l != NULL; l = l->l_next) {
		info.dlpi_name = l->l_name;
		info.dlpi_addr = l->l_addr;
		info.dlpi_phdr = l->l_phdr;
		info.dlpi_phnum = l->l_phnum;
		int ret = callback(&info, sizeof(dl_phdr_info), data);
		if (ret != 0) {
			return ret;
		}
	}

	return 0;
}
