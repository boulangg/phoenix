#pragma once

#include <elf64.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct link_map
	{
		Elf64_Addr l_addr;			/* Difference between the address in the ELF file and the addresses in memory.  */
		char* l_name;				/* Absolute file name object was found in.  */
		Elf64_Dyn* l_ld;			/* Dynamic section of the shared object.  */
		struct link_map* l_next;	/* Chain of loaded objects.  */
		struct link_map* l_prev;
		const Elf64_Phdr* l_phdr;	/* Pointer to program header table in core.  */
		Elf64_Addr l_entry;			/* Entry point location.  */
		Elf64_Half l_phnum;			/* Number of program header entries.  */
		Elf64_Half l_ldnum;			/* Number of dynamic segment entries.  */
	} link_map;

	extern struct link_map __main_link_map;

	struct link_map* _dl_find_dso_for_object(const Elf64_Addr addr);
	bool _dl_addr_inside_object(struct link_map* l, const Elf64_Addr addr);

	void init_dl();

#ifdef __cplusplus
}
#endif