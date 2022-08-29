#include "Debug.hpp"

#include <cstdio>
#include <stdarg.h>

#include <core/Console.hpp>

#define BUFFER_SIZE 1024

void printk(const char* format, ...)
{
	char tmp[BUFFER_SIZE];
	unsigned long int size = BUFFER_SIZE;
	va_list vl;
	vsnprintf(tmp, size, format, vl);
	Console::write(tmp);
}
