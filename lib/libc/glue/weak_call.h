#ifndef __WEAK_CALL_H__
#define __WEAK_CALL_H__

#include "stddef.h"

void *sbrk(ptrdiff_t increment) __attribute__((weak));
void exit(int status) __attribute__((weak));

#endif // __WEAK_CALL_H__
