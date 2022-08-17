#include "stdlib.h"

#include "syscall/syscall.h"

void abort(void) {
	sys_exit(-1);
	__builtin_unreachable();
}
