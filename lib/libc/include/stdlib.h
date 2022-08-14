/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef _KERNEL_STDLIB_H_
#define _KERNEL_STDLIB_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// Dynamic memory management
void* calloc(size_t num, size_t size);
void free(void* ptr);
void* malloc(size_t size);
void* realloc(void* ptr, size_t size);
int atexit(void(*func)());
void exit(int exit_code);

#ifdef __cplusplus
}
#endif

#endif /* _KERNEL_STDLIB_H_ */
