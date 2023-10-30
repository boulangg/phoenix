#pragma once

#include <cstddef>


void printk(const char* format, ...);

#ifndef offsetof
#define offsetof(st, m) __builtin_offsetof(st, m)
#endif

template <std::size_t ExpectedSize, std::size_t RealSize>
struct check_size
{
	static_assert(ExpectedSize == RealSize, "Incorrect size");
};

template <class T, class S, std::size_t ExpectedSize, std::size_t RealSize = offsetof(T, S)>
struct check_offset
{
	static_assert(ExpectedSize == RealSize, "Incorrect size");
};
