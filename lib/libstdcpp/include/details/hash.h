/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include <cstddef>

namespace std {

// hash
template <class K, class Arg>
struct __hash_base
{
    typedef K result_type;
    typedef Arg argument_type;
};

template <class Key>
struct hash;

// Multiplicative fibonnaci hashing
#if __WORDSIZE == 64
#define _HASH_FIBONNACI_CONSTANT 11400714819323198485ull
#else
#define _HASH_FIBONNACI_CONSTANT 2654435769ul
#endif

#define _default_integral_hash(T)                                                                                      \
    template <>                                                                                                        \
    struct hash<T> : public __hash_base<size_t, T>                                                                     \
    {                                                                                                                  \
        size_t operator()(T val) const noexcept                                                                        \
        {                                                                                                              \
            return (size_t)(val)*_HASH_FIBONNACI_CONSTANT;                                                             \
        }                                                                                                              \
    };

_default_integral_hash(bool);
_default_integral_hash(char);
_default_integral_hash(unsigned char);
_default_integral_hash(short);
_default_integral_hash(unsigned short);
_default_integral_hash(int);
_default_integral_hash(unsigned int);
_default_integral_hash(long);
_default_integral_hash(unsigned long);
_default_integral_hash(long long);
_default_integral_hash(unsigned long long);

#undef _HASH_FIBONNACI_CONSTANT

template <class T>
struct hash<T*> : public __hash_base<size_t, T*>
{
    size_t operator()(T* ptr) const noexcept
    {
        return hash<size_t>()((size_t)ptr);
    }
};

#undef _default_integral_hash

}
