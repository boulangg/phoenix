/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef __WEAK_CALL_H__
#define __WEAK_CALL_H__

#include "stddef.h"

void *sbrk(ptrdiff_t increment) __attribute__((weak));
void exit(int status) __attribute__((weak));

#endif // __WEAK_CALL_H__
