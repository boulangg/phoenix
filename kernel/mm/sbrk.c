/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include <stddef.h>
#include <errno.h>

extern char _kernel_mem_heap[];
extern char _kernel_mem_heap_end[];
static char* kernel_mem_head_curr = _kernel_mem_heap;


void *sbrk(ptrdiff_t diff)
{
	char* old_brk = kernel_mem_head_curr;
	char* new_brk = old_brk+diff;
	if ((new_brk > _kernel_mem_heap_end) || (new_brk < old_brk)) {
		errno = ENOMEM;
		return (void*)(-1);
	}
	return old_brk;
}
