/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef _STRING_H_
#define _STRING_H_

#include <def/def_null.h>
#include <def/def_restrict.h>
#include <def/def_size.h>

#ifdef __cplusplus
extern "C" {
#endif

void* memcpy(void* restrict dst, const void* restrict src, size_t count);
void* memmove(void* dst, const void* src, size_t num);
char* strcpy(char* dst, const char* src);
char* strncpy(char* dst, const char* src, size_t num);

char* strcat(char* dst, const char* src);
char* strncat(char* dst, const char* src, size_t num);

int memcmp(const void* ptr1, const void* ptr2, size_t num);
int strcmp(const char* str1, const char* str2);
int strcoll(const char* str1, const char* str2);
int strncmp(const char* str1, const char* str2, size_t num);
size_t strxfrm(char* dst, const char* src, size_t num);

void* memchr(const void* ptr, int value, size_t num);
char* strchr(const char* str, int character);
size_t strcspn(const char* str1, const char* str2);
char* strpbrk(const char* str1, const char* str2);
char* strrchr(const char* str, int character);
size_t strspn(const char* str1, const char* str2);
char* strstr(const char* str1, const char* str2);
char* strtok(char* str, const char* delims);

void* memset(void* dst, int ch, size_t count);
char* strerror(int errnum);
size_t strlen(const char* str);

#ifdef __cplusplus
}
#endif

#endif /* _STRING_H_ */
