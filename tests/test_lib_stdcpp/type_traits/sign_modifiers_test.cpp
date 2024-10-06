/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #include <def/type_traits/sign_modifiers.h>

#include <def/type_traits/_is_same.h>

namespace libstdcpp {
namespace type_traits {
enum E
{
};

//make_signed
namespace make_signed {
static_assert(std::is_same_v<std::make_signed_t<unsigned char>, signed char> == true);
static_assert(std::is_same_v<std::make_signed_t<unsigned int>, signed int> == true);
static_assert(std::is_same_v<std::make_signed_t<volatile unsigned long>, volatile signed long> == true);
// TODO implem: make_signed for enum for make_signed test
//static_assert(std::is_same_v<std::make_signed_t<E>, signed short> == true);
}

// make_unsigned
namespace make_unsigned {
static_assert(std::is_same_v<std::make_unsigned_t<char>, unsigned char> == true);
static_assert(std::is_same_v<std::make_unsigned_t<int>, unsigned int> == true);
static_assert(std::is_same_v<std::make_unsigned_t<volatile long>, volatile unsigned long> == true);
// TODO implem: make_unsigned for enum for make_unsigned test
// static_assert(std::is_same_v<std::make_signed_t<E>, signed short> == true);
}
}
}
