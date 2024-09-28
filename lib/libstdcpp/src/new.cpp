/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include <cstdlib>
#include <new>

void* operator new(std::size_t size)
{
    if (size == 0) {
        size = 1;
    }

    auto res = std::malloc(size);
    if (res == nullptr) {
        throw std::bad_alloc();
    }
    return res;
}

void* operator new[](std::size_t size)
{
    if (size == 0) {
        size = 1;
    }

    auto res = std::malloc(size);
    if (res == nullptr) {
        throw std::bad_alloc();
    }
    return res;
}

void* operator new(std::size_t, void* ptr) noexcept
{
    return ptr;
}

void* operator new[](std::size_t, void* ptr) noexcept
{
    return ptr;
}

void operator delete(void* ptr) noexcept
{
    std::free(ptr);
}

void operator delete[](void* ptr) noexcept
{
    std::free(ptr);
}

void operator delete(void* ptr, std::size_t) noexcept
{
    std::free(ptr);
}

void operator delete[](void* ptr, std::size_t) noexcept
{
    std::free(ptr);
}

extern "C" void __cxa_throw_bad_array_new_length()
{
    throw std::bad_array_new_length();
}
