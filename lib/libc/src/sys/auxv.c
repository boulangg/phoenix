#include "sys/auxv.h"

#include <stddef.h>

extern auxv_t* __auxv_ptr;

uint64_t getauxval(uint64_t __type)
{
	size_t i = 0;
	while (__auxv_ptr[i].a_type != AT_NULL) {
		if (__auxv_ptr[i].a_type == __type) {
			return __auxv_ptr[i].a_un.a_val;
		}
		i++;
	}
	return 0;
}

