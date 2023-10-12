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
    return malloc(size);
}

void* operator new[](std::size_t size)
{
    if (size == 0) {
        size = 1;
    }
    return malloc(size);
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
    free(ptr);
}

void operator delete[](void* ptr) noexcept
{
    free(ptr);
}

void operator delete(void* ptr, std::size_t) noexcept
{
    free(ptr);
}

void operator delete[](void* ptr, std::size_t) noexcept
{
    free(ptr);
}
