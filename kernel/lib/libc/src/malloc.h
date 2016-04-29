/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef MALLOC_H_
#define MALLOC_H_
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void* malloc(size_t);
void free(void*);
void* calloc(size_t,size_t);
void* realloc(void* ptr, size_t size);

#ifdef __cplusplus
}
#endif


#endif // MALLOC_H_

