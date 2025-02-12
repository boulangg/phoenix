/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #include <def/type_traits/references.h>

#include <def/type_traits/_is_same.h>
#include <def/type_traits/type_categories.h>

namespace libstdcpp {
namespace type_traits {
// remove_reference
namespace remove_reference {
static_assert(std::is_same_v<std::remove_reference_t<int>, int> == true);
static_assert(std::is_same_v<std::remove_reference_t<int&>, int> == true);
static_assert(std::is_same_v<std::remove_reference_t<int&&>, int> == true);
static_assert(std::is_same_v<std::remove_reference_t<const int&>, const int> == true);
}

// add_reference
namespace add_reference {
using non_ref = int;
using l_ref = std::add_lvalue_reference_t<non_ref>;
using r_ref = std::add_rvalue_reference_t<non_ref>;

static_assert(std::is_lvalue_reference_v<non_ref> == false);
static_assert(std::is_lvalue_reference_v<l_ref> == true);
static_assert(std::is_lvalue_reference_v<r_ref> == false);
static_assert(std::is_rvalue_reference_v<non_ref> == false);
static_assert(std::is_rvalue_reference_v<l_ref> == false);
static_assert(std::is_rvalue_reference_v<r_ref> == true);

using void_ref = std::add_lvalue_reference_t<void>;
static_assert(std::is_reference_v<void_ref> == false);
}
}
}
