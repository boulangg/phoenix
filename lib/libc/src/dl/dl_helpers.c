#include "dl_helpers.h"

#include <sys/auxv.h>

struct link_map __main_link_map;

void init_dl()
{
	__main_link_map.l_addr = getauxval(AT_PHDR) & ~(getauxval(AT_PAGESZ) - 1);
	__main_link_map.l_name = "";
	__main_link_map.l_next = NULL;
	__main_link_map.l_prev = NULL;
	__main_link_map.l_phdr = (void*)getauxval(AT_PHDR);
	__main_link_map.l_entry = getauxval(AT_ENTRY);
	__main_link_map.l_phnum = getauxval(AT_PHNUM);

	for (Elf64_Half i = 0; i < __main_link_map.l_phnum; i++) {
		if (__main_link_map.l_phdr[i].p_type == PT_DYNAMIC) {
			__main_link_map.l_ld = (void*)(__main_link_map.l_addr + __main_link_map.l_phdr[i].p_vaddr);
			Elf64_Dyn* dynShHdr = __main_link_map.l_ld;
			Elf64_Half ldNum = 0;
			while (dynShHdr[ldNum].d_tag != DT_NULL) {
				i++;
			}
			__main_link_map.l_ldnum = ldNum;
			break;
		}
	}

}

struct link_map* _dl_find_dso_for_object(const Elf64_Addr addr)
{
	for (struct link_map* l = &__main_link_map; l != NULL; l = l->l_next) {
		if (_dl_addr_inside_object(l, addr)) {
			return l;
		}
	}
	return NULL;
}

bool _dl_addr_inside_object(struct link_map* l, const Elf64_Addr addr)
{
	uint64_t offset = 0;//l->l_addr
	for (Elf64_Half i = 0; i < l->l_phnum; i++) {
		if (l->l_phdr[i].p_type == PT_LOAD
			&& offset + l->l_phdr[i].p_vaddr < addr
			&& addr < offset + l->l_phdr[i].p_vaddr + l->l_phdr[i].p_memsz) {
			return true;
		}
	}
	return false;
}

