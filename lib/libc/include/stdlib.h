/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef _KERNEL_STDLIB_H_
#define _KERNEL_STDLIB_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/// Standard C
// Dynamic memory management
void* calloc(size_t num, size_t size);
void free(void* ptr);
void* malloc(size_t size);
void* realloc(void* ptr, size_t size);
int atexit(void (*func)());
void exit(int exit_code) __attribute__((noreturn));

// Misc
int abs(int value);

// Other
#define EXIT_SUCCESS 0
#define EXIT_FAILURE -1
void abort(void) __attribute__((__noreturn__));

/// Linux methods
// Environment variables
extern char** environ;
const char* getenv(const char* varName);

int clearenv(void);
// int putenv(char *string);
// int setenv(const char *name, const char *value, int overwrite);
// int unsetenv(const char *name);

#ifdef __cplusplus
}
#endif

#endif /* _KERNEL_STDLIB_H_ */
