#include <stddef.h>
#include <errno.h>

extern char kernel_mem_heap[];
extern char kernel_mem_heap_end[];
static char* kernel_mem_head_curr = kernel_mem_heap;


void *sbrk(ptrdiff_t diff)
{
	char* old_brk = kernel_mem_head_curr;
	char* new_brk = old_brk+diff;
	if ((new_brk > kernel_mem_heap_end) || (new_brk < old_brk)) {
		errno = ENOMEM;
		return (void*)(-1);
	}
	return old_brk;
}
