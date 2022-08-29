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

void* memmove(void* dst, const void* src, size_t num)
{
	const char* p = (const char*)src;
	char* q = (char*)dst;
	if (p < q) {
		for (size_t i = 0; i < num; ++i) {
			q[i] = p[i];
		}
	} else if (p > q) {
		for (size_t i = 0; i < num; ++i) {
			q[num - 1 + i] = p[num - 1 + i];
		}
	}
	return dst;
}

int memcmp(const void* ptr1, const void* ptr2, size_t num)
{
	const char* p = (const char*)ptr1;
	const char* q = (const char*)ptr2;
	for (size_t i = 0; i < num; ++i) {
		if (p[i] != q[i]) {
			return p[i] - q[i];
		}
	}
	return 0;
}

void* memchr(const void* ptr, int value, size_t num)
{
	unsigned char val = (unsigned char)value & 0xFF;
	unsigned char* p = (unsigned char*)ptr;
	for (size_t i = 0; i < num; ++i) {
		if (p[i] == val) {
			return (void*)(p + i);
		}
	}
	return NULL;
}

void* memset(void* dst, int ch, size_t count)
{
	char* q = (char*)dst;
	for (size_t i = 0; i < count; ++i) {
		*q++ = ch;
	}
	return dst;
}
