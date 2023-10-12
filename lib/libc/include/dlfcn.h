/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define RTLD_LAZY   0x1
#define RTLD_NOW    0x2
#define RTLD_LOCAL  0x4
#define RTLD_GLOBAL 0x8

void* dlopen(const char* filename, int flag);
void* dlsym(void* handle, const char* symbol);
int dlclose(void* handle);
char* dlerror(void);

#ifdef __cplusplus
}
#endif
