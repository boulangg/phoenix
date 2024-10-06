/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <cstddef>
#include <stdfloat>

#include <def/type_traits/const_volatility_specifiers.h>
#include <def/type_traits/integral_constant.h>
#include <def/type_traits/_is_const.h>
#include <def/type_traits/_is_same.h>

namespace std {
// Primary type categories
template <class T>
struct is_void : std::is_same<void, typename std::remove_cv_t<T>>
{};
template <class T>
constexpr bool is_void_v = is_void<T>::value;

template <class T>
struct is_null_pointer : std::is_same<std::nullptr_t, std::remove_cv_t<T>>
{};
template <class T>
constexpr bool is_null_pointer_v = is_null_pointer<T>::value;

template <class T>
    struct is_integral : std::bool_constant < requires(T t, T* p, void (*f)(T)) // T* parameter excludes reference types
{
    reinterpret_cast<T>(t); // Exclude class types
    f(0);                   // Exclude enumeration types
    p + t;                  // Exclude everything not yet excluded but integral types
} > {};
template <class T>
constexpr bool is_integral_v = is_integral<T>::value;

template <class T>
struct is_floating_point :
    std::integral_constant<bool,
                           // Note: standard floating-point types
                           std::is_same<float, typename std::remove_cv<T>::type>::value ||
                               std::is_same<double, typename std::remove_cv<T>::type>::value ||
                               std::is_same<long double, typename std::remove_cv<T>::type>::value ||
                               // TODO Note: extended floating-point types (C++23, if supported)
                               std::is_same<std::float16_t, typename std::remove_cv<T>::type>::value ||
                               std::is_same<std::float32_t, typename std::remove_cv<T>::type>::value ||
                               std::is_same<std::float64_t, typename std::remove_cv<T>::type>::value ||
                               std::is_same<std::float128_t, typename std::remove_cv<T>::type>::value ||
                               std::is_same<std::bfloat16_t, typename std::remove_cv<T>::type>::value>
{};
template <class T>
constexpr bool is_floating_point_v = is_floating_point<T>::value;

template <class T>
struct is_array : std::false_type
{};
template <class T>
struct is_array<T[]> : std::true_type
{};
template <class T, std::size_t N>
struct is_array<T[N]> : std::true_type
{};
template <class T>
constexpr bool is_array_v = is_array<T>::value;

template <class T>
struct is_enum : std::bool_constant<__is_enum(T)>
{};
template <class T>
inline constexpr bool is_enum_v = is_enum<T>::value;

template <class T>
struct is_union : std::bool_constant<__is_union(T)>
{};
template <class T>
inline constexpr bool is_union_v = is_union<T>::value;

template <class T>
struct is_class : std::bool_constant<__is_class(T)>
{};
template <class T>
inline constexpr bool is_class_v = is_class<T>::value;

// Composite type categories (forward)
template <class T>
struct is_reference : std::false_type
{};
template <class T>
struct is_reference<T&> : std::true_type
{};
template <class T>
struct is_reference<T&&> : std::true_type
{};
template <class T>
constexpr bool is_reference_v = is_reference<T>::value;

// Primary type categories (next)
template <class T>
struct is_function : std::integral_constant<bool, !std::is_const<const T>::value && !std::is_reference<T>::value>
{};
template <class T>
constexpr bool is_function_v = is_function<T>::value;

template <class T>
struct is_pointer : std::false_type
{};
template <class T>
struct is_pointer<T*> : std::true_type
{};
template <class T>
struct is_pointer<T* const> : std::true_type
{};
template <class T>
struct is_pointer<T* volatile> : std::true_type
{};
template <class T>
struct is_pointer<T* const volatile> : std::true_type
{};
template <class T>
constexpr bool is_pointer_v = is_pointer<T>::value;

template <class T>
struct is_lvalue_reference : std::false_type
{};
template <class T>
struct is_lvalue_reference<T&> : std::true_type
{};
template <class T>
constexpr bool is_lvalue_reference_v = is_lvalue_reference<T>::value;

template <class T>
struct is_rvalue_reference : std::false_type
{};
template <class T>
struct is_rvalue_reference<T&&> : std::true_type
{};
template <class T>
constexpr bool is_rvalue_reference_v = is_rvalue_reference<T>::value;

template <class T>
struct is_member_function_pointer_helper : std::false_type
{};
template <class T, class U>
struct is_member_function_pointer_helper<T U::*> : std::is_function<T>
{};
template <class T>
struct is_member_function_pointer : is_member_function_pointer_helper<typename std::remove_cv<T>::type>
{};
template <class T>
constexpr bool is_member_function_pointer_v = is_member_function_pointer<T>::value;

// Composite type categories (forward)
template <class T>
struct is_member_pointer_helper : std::false_type
{};
template <class T, class U>
struct is_member_pointer_helper<T U::*> : std::true_type
{};
template <class T>
struct is_member_pointer : is_member_pointer_helper<typename std::remove_cv<T>::type>
{};
template <class T>
constexpr bool is_member_pointer_v = is_member_pointer<T>::value;

// Primary type categories (next)
template <class T>
struct is_member_object_pointer :
    std::integral_constant<bool, std::is_member_pointer<T>::value && !std::is_member_function_pointer<T>::value>
{};
template <class T>
constexpr bool is_member_object_pointer_v = is_member_object_pointer<T>::value;

// Composite type categories
template <class T>
struct is_arithmetic : std::integral_constant<bool, std::is_integral<T>::value || std::is_floating_point<T>::value>
{};
template <class T>
constexpr bool is_arithmetic_v = is_arithmetic<T>::value;

template <class T>
struct is_fundamental :
    std::integral_constant<bool,
                           std::is_arithmetic<T>::value || std::is_void<T>::value || std::is_null_pointer<T>::value>
{};
template <class T>
constexpr bool is_fundamental_v = is_fundamental<T>::value;

template <class T>
struct is_scalar :
    std::integral_constant<bool, std::is_arithmetic<T>::value || std::is_enum<T>::value || std::is_pointer<T>::value ||
                                     std::is_member_pointer<T>::value || std::is_null_pointer<T>::value>
{};
template <class T>
constexpr bool is_scalar_v = is_scalar<T>::value;

template <class T>
struct is_object :
    std::integral_constant<bool, std::is_scalar<T>::value || std::is_array<T>::value || std::is_union<T>::value ||
                                     std::is_class<T>::value>
{};
template <class T>
constexpr bool is_object_v = is_object<T>::value;

template <class T>
struct is_compound : std::integral_constant<bool, !std::is_fundamental<T>::value>
{};
template <class T>
constexpr bool is_compound_v = is_compound<T>::value;
}
