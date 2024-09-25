/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include <stdexcept>

#include <cstdlib>
#include <cstring>

namespace std {

namespace details {
RefCountedString::RefCountedString(const char* data) : _data(), _count(1)
{
    _data = (char*)(std::malloc(strlen(data) + 1));
    std::strncpy(_data, data, strlen(data));
}

RefCountedString::~RefCountedString()
{
    delete _data;
}

std::size_t RefCountedString::decrement()
{
    return --_count;
}
void RefCountedString::increment()
{
    ++_count;
}

const char* RefCountedString::what() const noexcept
{
    return _data;
}

}

logic_error::logic_error(const string& what_arg) : exception(), _msg(new details::RefCountedString{what_arg.c_str()}) {}

logic_error::logic_error(const char* what_arg) : exception(), _msg(new details::RefCountedString{what_arg}) {}

logic_error::logic_error(const logic_error& other) : exception(), _msg(other._msg)
{
    _msg->increment();
}

logic_error::~logic_error()
{
    if (_msg->decrement() == 0) {
        delete _msg;
    }
}

const char* logic_error::what() const noexcept
{
    return _msg->what();
}

domain_error::domain_error(const string& what_arg) : logic_error(what_arg) {}

domain_error::domain_error(const char* what_arg) : logic_error(what_arg) {}

invalid_argument::invalid_argument(const string& what_arg) : logic_error(what_arg) {}

invalid_argument::invalid_argument(const char* what_arg) : logic_error(what_arg) {}

length_error::length_error(const string& what_arg) : logic_error(what_arg) {}

length_error::length_error(const char* what_arg) : logic_error(what_arg) {}

out_of_range::out_of_range(const string& what_arg) : logic_error(what_arg) {}

out_of_range::out_of_range(const char* what_arg) : logic_error(what_arg) {}

runtime_error::runtime_error(const string& what_arg) :
    exception(), _msg(new details::RefCountedString{what_arg.c_str()})
{}

runtime_error::runtime_error(const char* what_arg) : exception(), _msg(new details::RefCountedString{what_arg}) {}

runtime_error::runtime_error(const runtime_error& other) : exception(), _msg(other._msg)
{
    _msg->increment();
}

runtime_error::~runtime_error()
{
    if (_msg->decrement() == 0) {
        delete _msg;
    }
}

const char* runtime_error::what() const noexcept
{
    return _msg->what();
}

range_error::range_error(const string& what_arg) : runtime_error(what_arg) {}

range_error::range_error(const char* what_arg) : runtime_error(what_arg) {}

overflow_error::overflow_error(const string& what_arg) : runtime_error(what_arg) {}

overflow_error::overflow_error(const char* what_arg) : runtime_error(what_arg) {}

underflow_error::underflow_error(const string& what_arg) : runtime_error(what_arg) {}

underflow_error::underflow_error(const char* what_arg) : runtime_error(what_arg) {}

}
