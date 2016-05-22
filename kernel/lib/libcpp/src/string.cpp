/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include <string>
#include <utility>


size_t std::string::length(const char *s) {
    size_t len = 0;
    while(*s++!='\0')
        len++;
    return len;
}

std::string::string() {

}

std::string::string(const char *s): _size(length(s)),_capacity(std::nearest_power_2(_size+1)),_data(new char[_capacity]) {
    for(size_t i=0;i<_size;i++)
        _data[i]=s[i];
    _data[_size]='\0';
}

std::string::string(const string& s): _size(s._size),_capacity(s._capacity),_data(new char[_capacity]) {
    for(size_t i=0;i<_size+1;i++)
        _data[i] = s._data[i];
}

std::string::string(string&& s): _size(s._size), _capacity(s._capacity),_data(s._data) {
    s._data = nullptr;
    s._size =0;
    s._capacity=0;
}

std::string& std::string::operator+=(const string& str) {
    if(_size+str._size>_capacity){
        _capacity = std::nearest_power_2(_size+str._size+1);
        char * tmp = new char[_capacity];
        for(size_t i=0;i<_size;i++)
            tmp[i]= _data[i];
        delete[] _data;
    }
    for(size_t i=0;i<str._size;i++)
        _data[_size+i]=str._data[i];
    _size+=str._size;
    _data[_size] = '\0';
    return *this;
}

std::string& std::string::operator+=(const char* str) {
    size_t len = length(str);
    if(_size+len>_capacity){
        _capacity = std::nearest_power_2(_size+len+1);
        char * tmp = new char[_capacity];
        for(size_t i=0;i<_size;i++)
            tmp[i]= _data[i];
        delete[] _data;
    }
    for(size_t i=0;i<len;i++)
        _data[_size+i]=str[i];
    _size+=len;
    _data[_size] = '\0';
    return *this;
}

const char* std::string::c_str() const {
    return _data;
}

