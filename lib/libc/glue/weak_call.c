#include "weak_call.h"

void *sbrk(ptrdiff_t increment) {
	(void)increment;
	return 0;
}

void exit(int status) {
	(void)status;
	__builtin_unreachable();
}
