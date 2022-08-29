/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include <string.h>
#include <stdbool.h>

char* strcpy(char* dst, const char* src)
{
	size_t i = 0;
	do {
		dst[i] = src[i];
	} while (src[i++] != '\0');
	return dst;
}

char* strncpy(char* dst, const char* src, size_t num)
{
	size_t i = 0;
	do {
		dst[i] = src[i];
	} while ((src[i++] != '\0') && (i < num));
	do {
		dst[i++] = '\0';
	} while (i < num);
	return dst;
}

char* strcat(char* dst, const char* src)
{
	size_t i = 0;
	while (dst[i] != '\0') {
		++i;
	}
	size_t j = 0;
	do {
		dst[i++] = src[j];
	} while (src[j++] != '\0');
	return dst;
}

char* strncat(char* dst, const char* src, size_t num)
{
	size_t i = 0;
	while (dst[i] != '\0') {
		++i;
	}
	size_t j = 0;
	do {
		dst[i++] = src[j];
	} while ((src[j++] != '\0') && (j < num));
	dst[i] = '\0';
	return dst;
}

int strcmp(const char* str1, const char* str2)
{
	size_t i = 0;
	for (; (str1[i] != '\0') && (str2[i] != '\0'); ++i) {
		if (str1[i] != str2[i]) {
			return str1[i] - str2[i];
		}
	}
	return str1[i] - str2[i];
}

int strcoll(const char* str1, const char* str2)
{
// TODO not yet implemented
	(void)str1;
	(void)str2;
	return 0;
}

int strncmp(const char* str1, const char* str2, size_t num)
{
	size_t i = 0;
	for (; (str1[i] != '\0') && (str2[i] != '\0') && (i < num); ++i) {
		if (str1[i] != str2[i]) {
			return str1[i] - str2[i];
		}
	}
	if (i == num) {
		return 0;
	}
	return str1[i] - str2[i];
}

size_t strxfrm(char* dst, const char* src, size_t num)
{
// TODO not yet implemented
	(void)dst;
	(void)src;
	(void)num;
	return 0;
}

char* strchr(const char* str, int character)
{
	char val = (char)character;
	size_t i = 0;
	do {
		if (str[i] == val) {
			return (char*)str + i;
		}
	} while (str[i++] != '\0');
	return NULL;
}

size_t strcspn(const char* str1, const char* str2)
{
	size_t i = 0;
	for (; str1[i] != '\0'; ++i) {
		for (size_t j = 0; str2[j] != '\0'; ++j) {
			if (str1[i] == str2[j]) {
				return i;
			}
		}
	}
	return i;
}

char* strpbrk(const char* str1, const char* str2)
{
	size_t i = strcspn(str1, str2);
	if (str1[i] == '\0') {
		return NULL;
	} else {
		return (char*)str1 + i;
	}
}

char* strrchr(const char* str, int character)
{
	char val = (char)character;
	size_t i = 0;
	char* last = NULL;
	do {
		if (str[i] == val) {
			last = (char*)str + i;
		}
	} while (str[i++] != '\0');
	return last;
}

size_t strspn(const char* str1, const char* str2)
{
	size_t i = 0;
	do {
		bool contains = false;
		for (size_t j = 0; str2[j] != '\0'; ++j) {
			if (str1[i] == str2[j]) {
				contains = true;
				break;
			}
		}
		if (contains == false) {
			return i;
		}
	} while (str1[i++] != '\0');
	return i;
}

char* strstr(const char* str1, const char* str2)
{
// TODO not yet implemented
	(void)str1;
	(void)str2;
	return NULL;
}

char* strtok(char* str, const char* delims)
{
// TODO not yet implemented
	(void)str;
	(void)delims;
	return NULL;
}

size_t strlen(const char* str)
{
	size_t len = 0;
	while (str[len] != '\0') {
		++len;
	}
	return len;
}
