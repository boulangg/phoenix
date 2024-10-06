/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #include <def/type_traits/integral_constant.h>

#include <def/type_traits/_is_same.h>

namespace libstdcpp {
namespace type_traits {
// integral_constant
namespace integral_constant {
using two_t = std::integral_constant<int, 2>;
using four_t = std::integral_constant<int, 4>;

static_assert(not std::is_same_v<two_t, four_t>);
static_assert(two_t::value * 2 == four_t::value, "2*2 != 4");
static_assert(two_t() << 1 == four_t() >> 0, "2*2 != 4");

enum class E
{
    e1,
    e2
};
using c1 = std::integral_constant<E, E::e1>;
using c2 = std::integral_constant<E, E::e2>;
static_assert(c1::value != E::e2);
static_assert(c1() == E::e1);
static_assert(std::is_same_v<c2, c2>);

static_assert(std::true_type::value == true);
static_assert(std::true_type() == true);
static_assert(std::true_type{} == true);
}
}
}
