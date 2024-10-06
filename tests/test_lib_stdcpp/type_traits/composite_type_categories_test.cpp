/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #include <def/type_traits/type_categories.h>

#include <utility>

namespace libstdcpp {
namespace type_traits {

int foo();
class A
{};

// is_fundamental
namespace is_fundamental {
static_assert(std::is_fundamental_v<int> == true);
static_assert(std::is_fundamental_v<int&> == false);
static_assert(std::is_fundamental_v<int*> == false);
static_assert(std::is_fundamental_v<void> == true);
static_assert(std::is_fundamental_v<void*> == false);
static_assert(std::is_fundamental_v<float> == true);
static_assert(std::is_fundamental_v<float&> == false);
static_assert(std::is_fundamental_v<float*> == false);
static_assert(std::is_fundamental_v<std::nullptr_t> == true);
static_assert(std::is_fundamental_v<std::is_fundamental<int>> == false);

static_assert(std::is_fundamental_v<A> == false);
static_assert(std::is_fundamental_v<std::is_fundamental<A>::value_type>);
}

// is_arithmetic
namespace is_arithmetic {
enum class E : int
{
    e
};

static_assert(std::is_arithmetic_v<bool> == true);
static_assert(std::is_arithmetic_v<char> == true);
static_assert(std::is_arithmetic_v<char const> == true);
static_assert(std::is_arithmetic_v<int> == true);
static_assert(std::is_arithmetic_v<int const> == true);
static_assert(std::is_arithmetic_v<float> == true);
static_assert(std::is_arithmetic_v<float const> == true);
static_assert(std::is_arithmetic_v<std::size_t> == true);
static_assert(std::is_arithmetic_v<char&> == false);
static_assert(std::is_arithmetic_v<char*> == false);
static_assert(std::is_arithmetic_v<int&> == false);
static_assert(std::is_arithmetic_v<int*> == false);
static_assert(std::is_arithmetic_v<float&> == false);
static_assert(std::is_arithmetic_v<float*> == false);
static_assert(std::is_arithmetic_v<A> == false);
static_assert(std::is_arithmetic_v<E> == false);
static_assert(std::is_arithmetic_v<decltype(E::e)> == false);
// TODO implem: std::byte and std::atomic_int for is_arithmetic test
// static_assert(std::is_arithmetic_v<std::byte> == false);
// static_assert(std::is_arithmetic_v<std::atomic_int> == false);
}

// is_scalar
namespace is_scalar {
struct S
{
    int m;
    void foo();
} s;
int S::*mp = &S::m;
enum class E
{
    e
};

template <typename Head, typename... Tail>
void are_scalars(Head&& head, Tail&&... tail)
{
    using T = std::decay_t<decltype(head)>;
    static_assert(std::is_scalar_v<T> == true);

    if constexpr (sizeof...(Tail)) {
        are_scalars(std::forward<decltype(tail)>(tail)...);
    }
}

template <typename Head, typename... Tail>
void are_not_scalars(Head&& head, Tail&&... tail)
{
    using T = std::decay_t<decltype(head)>;
    static_assert(std::is_scalar_v<T> == false);

    if constexpr (sizeof...(Tail)) {
        are_not_scalars(std::forward<decltype(tail)>(tail)...);
    }
}

void test()
{
    are_scalars(42, 3.14, E::e, "str", mp, nullptr);
    are_not_scalars(s);
}
}

// is_object
namespace is_object {
static_assert(std::is_object_v<void> == false);
static_assert(std::is_object_v<int> == true);
static_assert(std::is_object_v<int&> == false);
static_assert(std::is_object_v<int*> == true);
static_assert(std::is_object_v<int*&> == false);
static_assert(std::is_object_v<A> == true);
static_assert(std::is_object_v<A&> == false);
static_assert(std::is_object_v<A*> == true);
static_assert(std::is_object_v<int()> == false);
static_assert(std::is_object_v<int(*)> == true);
static_assert(std::is_object_v<int(&)> == false);
}

// is_compound
namespace is_compound {
static_assert(std::is_compound_v<int> == false);
static_assert(std::is_compound_v<int*> == true);
static_assert(std::is_compound_v<int&> == true);

static_assert(std::is_compound_v<decltype(foo)> == true);
static_assert(std::is_compound_v<decltype(&foo)> == true);

static_assert(std::is_compound_v<char[100]> == true);

static_assert(std::is_compound_v<A> == true);

union U
{};
static_assert(std::is_compound_v<U> == true);

enum struct E
{
    e
};
static_assert(std::is_compound_v<E> == true);
static_assert(std::is_compound_v<decltype(E::e)> == true);

struct S
{
    int i : 8;
    int j;
    void foo();
};
static_assert(std::is_compound_v<decltype(S::i)> == false);
static_assert(std::is_compound_v<decltype(S::j)> == false);
static_assert(std::is_compound_v<decltype(&S::j)> == true);
static_assert(std::is_compound_v<decltype(&S::foo)> == true);
}

// is_reference
namespace is_reference {
static_assert(std::is_reference_v<A> == false);
static_assert(std::is_reference_v<A&> == true);
static_assert(std::is_reference_v<A&&> == true);
static_assert(std::is_reference_v<long> == false);
static_assert(std::is_reference_v<long&> == true);
static_assert(std::is_reference_v<long&&> == true);
static_assert(std::is_reference_v<double*> == false);
static_assert(std::is_reference_v<double*&> == true);
static_assert(std::is_reference_v<double*&&> == true);
}

}
}
