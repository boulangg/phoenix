/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #include <def/type_traits/logical_operations.h>

#include <string>

namespace libstdcpp {
namespace type_traits {

// conjunction
namespace conjunction {
template <typename T, typename... Ts>
constexpr bool all_types_are_same = std::conjunction_v<std::is_same<T, Ts>...>;

static_assert(all_types_are_same<int, int, int> == true);
static_assert(all_types_are_same<int, int&, int> == false);
}

// disjunction
namespace disjunction {
template <auto V1, decltype(V1) V2, typename T>
struct values_equal : std::bool_constant<V1 == V2>
{
    using type = T;
};

// default_type<T>::value is always true
template <typename T>
struct default_type : std::true_type
{
    using type = T;
};

// Now we can use disjunction like a switch statement:
template <int I>
using int_of_size = typename std::disjunction< //
    values_equal<I, 1, std::int8_t>,           //
    values_equal<I, 2, std::int16_t>,          //
    values_equal<I, 4, std::int32_t>,          //
    values_equal<I, 8, std::int64_t>,          //
    default_type<void>                         // must be last!
    >::type;

static_assert(sizeof(int_of_size<1>) == 1);
static_assert(sizeof(int_of_size<2>) == 2);
static_assert(sizeof(int_of_size<4>) == 4);
static_assert(sizeof(int_of_size<8>) == 8);
static_assert(std::is_same_v<int_of_size<13>, void> == true);

// checking if Foo is constructible from double will cause a hard error
struct Foo
{
    template <class T>
    struct sfinae_unfriendly_check
    {
        static_assert(!std::is_same_v<T, double> == true);
    };

    template <class T>
    Foo(T, sfinae_unfriendly_check<T> = {});
};

template <class... Ts>
struct first_constructible
{
    template <class T, class... Args>
    struct is_constructible_x : std::is_constructible<T, Args...>
    {
        using type = T;
    };

    struct fallback
    {
        static constexpr bool value = true;
        using type = void; // type to return if nothing is found
    };

    template <class... Args>
    using with = typename std::disjunction<is_constructible_x<Ts, Args...>..., fallback>::type;
};

// OK, is_constructible<Foo, double> not instantiated
static_assert(std::is_same_v<first_constructible<std::string, int, Foo>::with<double>, int> == true);

static_assert(std::is_same_v<first_constructible<std::string, int>::with<>, std::string> == true);
static_assert(std::is_same_v<first_constructible<std::string, int>::with<const char*>, std::string> == true);
static_assert(std::is_same_v<first_constructible<std::string, int>::with<void*>, void> == true);
}

// negation
namespace negation {
static_assert(std::negation_v<std::bool_constant<true>> == false);
static_assert(std::negation_v<std::bool_constant<false>> == true);
}

}
}
