#include "elf64.h"

size_t elf64_hash(const unsigned char* name)
{
	size_t h = 0, g;
	while (*name) {
		h = (h << 4) + *name++;
		if ((g = h & 0xf0000000) != 0) {
			h ^= g >> 24;
		}
		h &= 0x0fffffff;
	}
	return h;
}
