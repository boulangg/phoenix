/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include <algorithm>
#include <string>
#include <utility>

#include "utils.h"

#define DEFAULT_CAPACITY 4

namespace std {

size_t char_traits<char>::length(const char_type* s)
{
    size_t len = 0;
    while (*s++ != '\0')
        len++;
    return len;
}

void char_traits<char>::assign(char_type& r, const char_type& c) noexcept
{
    r = c;
}

char_traits<char>::char_type* char_traits<char>::copy(char_type* dest, const char_type* src, size_t n)
{
    for (size_t i = 0; i < n; ++i) {
        dest[i] = src[i];
    }
    return dest;
}

string::string() : _size(0), _capacity(DEFAULT_CAPACITY), _data(new char[_capacity])
{
    _data[0] = '\0';
}

string::string(const string& s) : _size(s._size), _capacity(s._capacity), _data(new char[_capacity])
{
    char_traits<char>::copy(_data, s._data, _size + 1);
}

string::string(string&& other) : string()
{
    swap(*this, other);
}

string::string(const char* s) : string(s, char_traits<char>::length(s)) {}

string::string(const char* s, size_t n) :
    _size(n), _capacity(utils::nearest_power_2(_size + 1)), _data(new char[_capacity])
{
    char_traits<char>::copy(_data, s, _size);
    _data[_size] = '\0';
}

string::string(size_t n, char c) : string()
{
    char* tmp = new char[n];
    for (size_t i = 0; i < n; i++) {
        tmp[i] = c;
    }
    append(tmp, n);
}

string::~string()
{
    delete[] _data;
}

string& string::operator=(string other)
{
    swap(*this, other);
    return *this;
}

string& string::operator=(const char* str)
{
    _size = char_traits<char>::length(str);
    _capacity = utils::nearest_power_2(_size + 1);
    delete[] _data;
    _data = new char[_capacity];
    char_traits<char>::copy(_data, str, _size + 1);
    return *this;
}

void swap(string& first, string& second)
{
    // enable ADL
    using std::swap;

    swap(first._size, second._size);
    swap(first._capacity, second._capacity);
    swap(first._data, second._data);
}

string& string::operator+=(const string& str)
{
    return append(str._data, str._size);
}

string& string::operator+=(const char* str)
{
    size_t len = char_traits<char>::length(str);
    return append(str, len);
}

char& string::operator[](int pos)
{
    return _data[pos];
}

const char& string::operator[](int pos) const
{
    return _data[pos];
}

bool string::empty() const noexcept
{
    return _size == 0;
}

size_t string::length() const
{
    return _size;
}

size_t string::capacity() const
{
    return _capacity;
}

string& string::append(const char* str, size_t len)
{
    if (_size + len > _capacity) {
        _capacity = utils::nearest_power_2(_size + len + 1);
        char* tmp = new char[_capacity];
        char_traits<char>::copy(tmp, _data, _size);
        delete[] _data;
        _data = tmp;
    }
    char_traits<char>::copy(_data + _size, str, len);
    _size += len;
    _data[_size] = '\0';
    return *this;
}

void string::push_back(char c)
{
    this->append(&c, 1);
}

int string::compare(const string& str) const
{
    size_t i = 0;
    while ((str._data[i] != '\0') && (_data[i] != '\0')) {
        if (str._data[i] != _data[i]) {
            return (_data[i] - str._data[i]);
        }
        ++i;
    }
    return (str._size - _size);
}

int string::compare(const char* s) const
{
    return compare(std::string(s));
}

bool string::starts_with(std::string sv) const noexcept
{
    return starts_with(sv.c_str());
}

bool string::starts_with(char ch) const noexcept
{
    if (empty()) {
        return false;
    }

    return _data[0] == ch;
}

bool string::starts_with(const char* s) const
{
    if (empty()) {
        return false;
    }

    for (std::size_t i = 0; s[i] != '\0'; i++) {
        if (s[i] != _data[i]) {
            return false;
        }
    }

    return true;
}

const char* string::c_str() const
{
    return _data;
}

string string::substr(size_t pos, size_t len) const
{
    string res(this->c_str() + pos, len);
    res._data[len] = '\0';
    return res;
}

string operator+(const string& lhs, const string& rhs)
{
    string ret(lhs);
    ret += rhs;
    return ret;
}

bool operator==(const string& lhs, const string& rhs)
{
    return lhs.compare(rhs) == 0;
}

bool operator==(const string& lhs, const char* rhs)
{
    return lhs.compare(string(rhs)) == 0;
}

std::strong_ordering operator<=>(const string& lhs, const string& rhs)
{
    auto compare = lhs.compare(rhs);
    if (compare > 0) {
        return std::strong_ordering::greater;
    } else if (compare < 0) {
        return std::strong_ordering::less;
    } else {
        return std::strong_ordering::equal;
    }
}

std::strong_ordering operator<=>(const string& lhs, const char* rhs)
{
    auto compare = lhs.compare(string(rhs));
    if (compare > 0) {
        return std::strong_ordering::greater;
    } else if (compare < 0) {
        return std::strong_ordering::less;
    } else {
        return std::strong_ordering::equal;
    }
}
}
