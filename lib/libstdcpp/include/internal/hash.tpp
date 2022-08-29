/*
 * Copyright (c) 2016-2021 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include <def/def_size.h>

namespace std
{

template<class K, class Arg>
struct __hash_base {
	typedef K   result_type;
	typedef Arg argument_type;
};

template<class T>
struct hash<T*> : public __hash_base<size_t, T*> {
	size_t operator()(T* ptr) const noexcept {
		size_t shift = 3; // TODO : use log2(1+sizeof(T))
		return (size_t)ptr >> shift;
	}
};

#define _default_integral_hash(T)                \
template<>                                       \
struct hash<T> : public __hash_base<size_t, T> { \
	size_t operator()(T val) const noexcept {    \
		return (size_t)(val);                    \
	}                                            \
};

_default_integral_hash(bool)
_default_integral_hash(char)

_default_integral_hash(unsigned char)
_default_integral_hash(short)
_default_integral_hash(unsigned short)
_default_integral_hash(int)
_default_integral_hash(unsigned int)
_default_integral_hash(long)
_default_integral_hash(unsigned long)
_default_integral_hash(long long)
_default_integral_hash(unsigned long long)

#undef _default_integral_hash

}
