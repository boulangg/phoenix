/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <cstdint>
#include <sys/types.h>

#ifdef __cplusplus
extern "C"
{
#endif

// C functions
void* sys_brk(void* addr);
// void* sys_mmap(void* addr, size_t length, int prot, int flags, int fd, off_t offset);

#ifdef __cplusplus
}
#endif
