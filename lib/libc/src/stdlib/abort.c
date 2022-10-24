#include "stdlib.h"

#include "syscall/syscall.h"
#include "stdio.h"

void abort(void)
{
	printf("Aborted\n");
	sys_exit(-1);
	__builtin_unreachable();
}
