/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #include <def/type_traits/misc_transformations.h>

#include <def/type_traits/_is_same.h>
#include <def/type_traits/references.h>

namespace libstdcpp {
namespace type_traits {
// decay
namespace decay {
template <typename T, typename U>
constexpr bool is_decay_equ = std::is_same_v<std::decay_t<T>, U>;

static_assert(std::is_same_v<std::decay_t<int>, int> == true);
static_assert(std::is_same_v<std::decay_t<int>, int> == true);
static_assert(std::is_same_v<std::decay_t<int>, float> == false);
static_assert(std::is_same_v<std::decay_t<int&>, int> == true);
static_assert(std::is_same_v<std::decay_t<int&&>, int> == true);
static_assert(std::is_same_v<std::decay_t<const int&>, int> == true);
static_assert(std::is_same_v<std::decay_t<int[2]>, int*> == true);
static_assert(std::is_same_v<std::decay_t<int[4][2]>, int*> == false);
static_assert(std::is_same_v<std::decay_t<int[4][2]>, int**> == false);
static_assert(std::is_same_v<std::decay_t<int[4][2]>, int (*)[2]> == true);
static_assert(std::is_same_v<std::decay_t<int(int)>, int (*)(int)>);
}

// remove_cvref
namespace remove_cvref {
static_assert(std::is_same_v<std::remove_cvref_t<int>, int> == true);
static_assert(std::is_same_v<std::remove_cvref_t<int&>, int> == true);
static_assert(std::is_same_v<std::remove_cvref_t<int&&>, int> == true);
static_assert(std::is_same_v<std::remove_cvref_t<const int&>, int> == true);
static_assert(std::is_same_v<std::remove_cvref_t<const int[2]>, int[2]> == true);
static_assert(std::is_same_v<std::remove_cvref_t<const int (&)[2]>, int[2]> == true);
static_assert(std::is_same_v<std::remove_cvref_t<int(int)>, int(int)> == true);
}
}

// enable_if
// TODO test: enable_if

// conditional
namespace conditional {
static_assert(std::is_same_v<std::conditional_t<true, int, double>, int> == true);
static_assert(std::is_same_v<std::conditional_t<false, int, double>, double> == true);
static_assert(std::is_same_v<std::conditional_t<sizeof(int) >= sizeof(double), int, double>, double> == true);
}

// common_type
namespace common_type {
static_assert(std::is_same_v<std::common_type_t<int, double>, double> == true);
static_assert(std::is_same_v<std::common_type_t<int, float>, int> == false);
static_assert(std::is_same_v<std::common_type_t<int, char>, int> == true);
}

// common_reference
namespace common_reference {
static_assert(std::is_same_v<std::details::__simple_common_reference<int&, int&>::type, int&> == true);
static_assert(std::is_same_v<std::details::__simple_common_reference_t<int&, int&>, int&> == true);
static_assert(std::is_same_v<std::details::__simple_common_reference<const int&, int&>::type, const int&> == true);
static_assert(std::is_same_v<std::details::__simple_common_reference<int&, const int&>::type, const int&> == true);

static_assert(std::is_same_v<std::details::__simple_common_reference<int&&, int&&>::type, int&&> == true);
static_assert(std::is_same_v<std::details::__simple_common_reference<const int&&, int&&>::type, const int&&> == true);

static_assert(
    std::is_same_v<int&,
                   std::common_reference_t<std::add_lvalue_reference_t<int&>, std::add_lvalue_reference_t<int>&,
                                           std::add_lvalue_reference_t<int>&&, const std::add_lvalue_reference_t<int>,
                                           std::add_lvalue_reference_t<int> const&>> == true);
// TODO test : basic_common_reference
}

// underlying_type
namespace underlying_type {
enum e1
{
};
enum class e2
{
};
enum class e3 : unsigned
{
};
enum class e4 : int
{
};

static_assert(std::is_same_v<std::underlying_type_t<e1>, int> == false);
static_assert(std::is_same_v<std::underlying_type_t<e2>, int> == true);
static_assert(std::is_same_v<std::underlying_type_t<e3>, int> == false);
static_assert(std::is_same_v<std::underlying_type_t<e4>, int> == true);
}

// invoke_result
// TODO test: invoke_result

// void
// TODO test: void

// type_identity
// TODO test: type_identity

// unwrap_reference
// TODO test: unwrap_reference
}
