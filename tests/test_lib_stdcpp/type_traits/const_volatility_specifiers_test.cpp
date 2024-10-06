/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #include <def/type_traits/const_volatility_specifiers.h>

#include <def/type_traits/_is_same.h>

namespace libstdcpp {
namespace type_traits {

// remove_cv
namespace remove_cv {
static_assert(std::is_same_v<std::remove_cv_t<int>, int> == true);
static_assert(std::is_same_v<std::remove_cv_t<const int>, int> == true);
static_assert(std::is_same_v<std::remove_cv_t<volatile int>, int> == true);
static_assert(std::is_same_v<std::remove_cv_t<const volatile int>, int> == true);
// remove_cv only works on types, not on pointers
static_assert(std::is_same_v<std::remove_cv_t<const volatile int*>, int*> == false);
static_assert(std::is_same_v<std::remove_cv_t<const volatile int*>, const volatile int*> == true);
static_assert(std::is_same_v<std::remove_cv_t<const int* volatile>, const int*> == true);
static_assert(std::is_same_v<std::remove_cv_t<int* const volatile>, int*> == true);
}

// add_cv
namespace add_cv {
static_assert(std::is_same_v<std::add_const_t<int>, const int> == true);
static_assert(std::is_same_v<std::add_volatile_t<int>, volatile int> == true);
static_assert(std::is_same_v<std::add_cv_t<int>, const volatile int> == true);
}

}
}
