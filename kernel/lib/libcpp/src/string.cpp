/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include <string>
#include <utility>

#define DEFAULT_CAPACITY 4

namespace std {

size_t char_traits<char>::length(const char_type* s){
	size_t len = 0;
	while(*s++!='\0')
		len++;
	return len;
}

void char_traits<char>::assign(char_type& r, const char_type& c) {
	r = c;
}

char_traits<char>::char_type* char_traits<char>::copy(char_type* dest, const char_type* src, size_t n) {
	for (size_t i = 0; i < n; ++i) {
		dest[i] = src[i];
	}
	return dest;
}

size_t string::length(const char *s) {
    size_t len = 0;
    while(*s++!='\0')
        len++;
    return len;
}

string::string(): _size(0), _capacity(DEFAULT_CAPACITY), _data(new char[_capacity]) {

}

string::string(const char *s): _size(length(s)),_capacity(nearest_power_2(_size+1)),_data(new char[_capacity]) {
	char_traits<char>::copy(_data, s, _size);
    _data[_size]='\0';
}

string::string(const string& s): _size(s._size),_capacity(s._capacity),_data(new char[_capacity]) {
	char_traits<char>::copy(_data, s._data, _size+1);
}

string::string(string&& s): _size(s._size), _capacity(s._capacity),_data(s._data) {
    s._data = nullptr;
    s._size =0;
    s._capacity=0;
}

string& string::operator+=(const string& str) {
	return append(str._data, str._size);
}

string& string::operator+=(const char* str) {
	size_t len = char_traits<char>::length(str);
	return append(str, len);
}

string& string::append(const char* str, size_t len) {
    if(_size+len>_capacity){
        _capacity = std::nearest_power_2(_size+len+1);
        char * tmp = new char[_capacity];
    	char_traits<char>::copy(tmp, _data, _size);
        delete[] _data;
        _data = tmp;
    }
	char_traits<char>::copy(_data+_size, str, len);
    _size+=len;
    _data[_size] = '\0';
    return *this;
}

const char* string::c_str() const {
    return _data;
}

}

