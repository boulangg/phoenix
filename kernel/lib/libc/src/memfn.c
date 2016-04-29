/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include <string.h>

void* memcpy(void* restrict dst, const void* restrict src, size_t count)
{
	const char* p = (const char*)src;
	char* q = (char*)dst;
	for (size_t i = 0; i < count; i++) {
		*q++ = *p++;
	}
	return dst;
}

void* memset(void* dst, int ch, size_t count)
{
	char* q = (char*)dst;
	for (size_t i = 0; i < count; i++) {
		*q++ = ch;
	}
	return dst;
}
