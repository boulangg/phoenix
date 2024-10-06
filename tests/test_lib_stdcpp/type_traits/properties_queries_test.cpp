/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #include <def/type_traits/property_queries.h>

#include <cstdint>

#include <def/type_traits/_is_same.h>
#include <def/type_traits/misc_transformations.h>

namespace libstdcpp {
namespace type_traits {

// alignment_of
namespace alignment_of {
struct A
{};
struct B
{
    std::int8_t p;
    std::int16_t q;
};
static_assert(std::alignment_of_v<A> == 1);
static_assert(std::alignment_of_v<B> == 2);
static_assert(std::alignment_of_v<int> == 4);
static_assert(std::alignment_of_v<double> == 8);
}

// rank
namespace rank {
static_assert(std::rank<int>{} == 0);
static_assert(std::rank<int[5]>{} == 1);
static_assert(std::rank<int[5][5]>{} == 2);
static_assert(std::rank<int[][5][5]>{} == 3);

void test()
{
    [[maybe_unused]] int ary[][3] = {{1, 2, 3}};

    // The rank of reference type, e.g., ary[0], that is int(&)[3], is 0:
    static_assert(std::rank_v<decltype(ary[0])> == 0);
    static_assert(std::is_same_v<decltype(ary[0]), int(&)[3]>);

    // The solution is to remove the reference type.
    static_assert(std::rank_v<std::remove_cvref_t<decltype(ary[0])>> == 1);
}
}

// extent
namespace extent {
static_assert(std::extent_v<int[3]> == 3);    // default dimension is 0
static_assert(std::extent_v<int[3], 0> == 3); // the same as above
static_assert(std::extent_v<int[3][4], 0> == 3);
static_assert(std::extent_v<int[3][4], 1> == 4);
static_assert(std::extent_v<int[3][4], 2> == 0);
static_assert(std::extent_v<int[]> == 0);

void test()
{
    const auto ext = std::extent<int['*']>{};
    static_assert(ext == 42); // with implicit conversion to std::size_t

    const int ints[]{1, 2, 3, 4};
    static_assert(std::extent_v<decltype(ints)> == 4); // array size

    [[maybe_unused]] int ary[][3] = {{1, 2, 3}};

    // ary[0] is of type reference to 'int[3]', so, the extent
    // cannot be calculated correctly and it returns 0
    static_assert(std::is_same_v<decltype(ary[0]), int(&)[3]> == true);
    static_assert(std::extent_v<decltype(ary[0])> == 0);

    // removing reference gives correct extent value 3
    static_assert(std::extent_v<std::remove_cvref_t<decltype(ary[0])>> == 3);
}
}

}
}
