/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include <exception>

#include "helpers/exception_handling.h"
#include <stdlib.h>

namespace std {

void terminate() noexcept
{
    abort();
}

exception_ptr::exception_ptr(const exception_ptr& other) noexcept : _ptr(other._ptr)
{
    std::details::__exc_increment_refCount(_ptr);
}

exception_ptr& exception_ptr::operator=(const exception_ptr& other)
{
    if (_ptr != other._ptr) {
        std::details::__exc_increment_refCount(other._ptr);
        std::details::__exc_decrement_refcount(_ptr);
        _ptr = other._ptr;
    }
    return *this;
}

exception_ptr::~exception_ptr()
{
    std::details::__exc_decrement_refcount(_ptr);
}

exception_ptr::operator bool() const
{
    return _ptr != nullptr;
}

bool operator==(const exception_ptr& x, const exception_ptr& y)
{
    return x._ptr == y._ptr;
}

exception_ptr current_exception() noexcept
{
    return exception_ptr(std::details::__exc_get_cuurent_ptr());
}

void rethrow_exception(exception_ptr p)
{
    if (p == nullptr) {
        terminate();
    }
    std::details::__exc_rethrow_exception(p._ptr);
}

exception::exception() noexcept {}

exception::exception(const exception&) noexcept {}

exception& exception::operator=(const exception&) noexcept
{
    return *this;
}

exception::~exception() {}

const char* exception::what() const noexcept
{
    return "std::exception";
}

const char* bad_exception::what() const noexcept
{
    return "std::bad_exception";
}

nested_exception::nested_exception() noexcept : _ptr(std::current_exception()) {}

void nested_exception::rethrow_nested() const
{
    if (_ptr == nullptr) {
        terminate();
    }
    rethrow_exception(_ptr);
}

exception_ptr nested_exception::nested_ptr() const noexcept
{
    return _ptr;
}

}
