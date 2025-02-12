/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <cstddef>
#include <def/type_traits/integral_constant.h>
#include <def/type_traits/type_categories.h>
#include <def/utility_declval.h>
#include <def/type_traits/logical_operations.h>

namespace std {
// is_constructible
template <class T, class... Args>
struct is_constructible : bool_constant<__is_constructible(T, Args...)>
{};
template <class T, class... Args>
inline constexpr bool is_constructible_v = is_constructible<T, Args...>::value;

template <class T, class... Args>
struct is_trivially_constructible : std::bool_constant<__is_trivially_constructible(T, Args...)>
{};
template <class T, class... Args>
inline constexpr bool is_trivially_constructible_v = is_trivially_constructible<T, Args...>::value;

namespace detail {
template <bool, bool, class T, class... Args>
struct _is_nothrow_constructible;
template <class T, class... Args>
struct _is_nothrow_constructible</*is constructible*/ true, /*is reference*/ false, T, Args...> :
    public integral_constant<bool, noexcept(T(declval<Args>()...))>
{};
template <class T>
void __implicit_conversion_to(T) noexcept
{}
template <class T, class Arg>
struct _is_nothrow_constructible</*is constructible*/ true, /*is reference*/ true, T, Arg> :
    public integral_constant<bool, noexcept(__implicit_conversion_to<T>(declval<Arg>()))>
{};
template <class T, bool IsRef, class... Args>
struct _is_nothrow_constructible</*is constructible*/ false, IsRef, T, Args...> : public false_type
{};
}

template <class T, class... Args>
struct is_nothrow_constructible :
    detail::_is_nothrow_constructible<is_constructible<T, Args...>::value, is_reference<T>::value, T, Args...>
{};

template <class T, size_t N>
struct is_nothrow_constructible<T[N]> :
    detail::_is_nothrow_constructible<is_constructible<T>::value, is_reference<T>::value, T>
{};
template <class T, class... Args>
inline constexpr bool is_nothrow_constructible_v = is_nothrow_constructible<T, Args...>::value;

// is_default_constructible
template <class T>
struct is_default_constructible : std::is_constructible<T>
{};
template <class T>
inline constexpr bool is_default_constructible_v = is_default_constructible<T>::value;

template <class T>
struct is_trivially_default_constructible : std::is_trivially_constructible<T>
{};
template <class T>
inline constexpr bool is_trivially_default_constructible_v = is_trivially_default_constructible<T>::value;

template <class T>
struct is_nothrow_default_constructible : std::is_nothrow_constructible<T>
{};
template <class T>
inline constexpr bool is_nothrow_default_constructible_v = is_nothrow_default_constructible<T>::value;

// is_copy_constructible
template <class T>
struct is_copy_constructible :
    std::is_constructible<T, typename std::add_lvalue_reference<typename std::add_const<T>::type>::type>
{};
template <class T>
inline constexpr bool is_copy_constructible_v = is_copy_constructible<T>::value;

template <class T>
struct is_trivially_copy_constructible :
    std::is_trivially_constructible<T, typename std::add_lvalue_reference<typename std::add_const<T>::type>::type>
{};
template <class T>
inline constexpr bool is_trivially_copy_constructible_v = is_trivially_copy_constructible<T>::value;

template <class T>
struct is_nothrow_copy_constructible :
    std::is_nothrow_constructible<T, typename std::add_lvalue_reference<typename std::add_const<T>::type>::type>
{};
template <class T>
inline constexpr bool is_nothrow_copy_constructible_v = is_nothrow_copy_constructible<T>::value;

// is_move_constructible
template <class T>
struct is_move_constructible : std::is_constructible<T, typename std::add_rvalue_reference<T>::type>
{};
template <class T>
inline constexpr bool is_move_constructible_v = is_move_constructible<T>::value;

template <class T>
struct is_trivially_move_constructible : std::is_trivially_constructible<T, typename std::add_rvalue_reference<T>::type>
{};
template <class T>
inline constexpr bool is_trivially_move_constructible_v = is_trivially_move_constructible<T>::value;

template <class T>
struct is_nothrow_move_constructible : std::is_nothrow_constructible<T, typename std::add_rvalue_reference<T>::type>
{};
template <class T>
inline constexpr bool is_nothrow_move_constructible_v = is_nothrow_move_constructible<T>::value;

// is_assignable
template <class T, class U>
struct is_assignable : bool_constant<__is_assignable(T, U)>
{};
template <class T, class U>
inline constexpr bool is_assignable_v = is_assignable<T, U>::value;

template <class T, class U>
struct is_trivially_assignable : std::bool_constant<__is_trivially_assignable(T, U)>
{};
template <class T, class U>
inline constexpr bool is_trivially_assignable_v = is_trivially_assignable<T, U>::value;

namespace detail {
template <bool, class T, class U>
struct _is_nothrow_assignable;
template <class T, class U>
struct _is_nothrow_assignable<false, T, U> : public false_type
{};
template <class T, class U>
struct _is_nothrow_assignable<true, T, U> : public integral_constant<bool, noexcept(declval<T>() = declval<U>())>
{};
}

template <class T, class U>
struct is_nothrow_assignable :
    std::bool_constant<detail::_is_nothrow_assignable<is_assignable<T, U>::value, T, U>::value>
{};
template <class T, class U>
inline constexpr bool is_nothrow_assignable_v = is_nothrow_assignable<T, U>::value;

// is_copy_assignable
template <class T>
struct is_copy_assignable :
    std::is_assignable<typename std::add_lvalue_reference<T>::type, typename std::add_lvalue_reference<const T>::type>
{};
template <class T>
inline constexpr bool is_copy_assignable_v = is_copy_assignable<T>::value;

template <class T>
struct is_trivially_copy_assignable :
    std::is_trivially_assignable<typename std::add_lvalue_reference<T>::type,
                                 typename std::add_lvalue_reference<const T>::type>
{};
template <class T>
inline constexpr bool is_trivially_copy_assignable_v = is_trivially_copy_assignable<T>::value;

template <class T>
struct is_nothrow_copy_assignable :
    std::is_nothrow_assignable<typename std::add_lvalue_reference<T>::type,
                               typename std::add_lvalue_reference<const T>::type>
{};
template <class T>
inline constexpr bool is_nothrow_copy_assignable_v = is_nothrow_copy_assignable<T>::value;

// is_move_assignable
template <class T>
struct is_move_assignable :
    std::is_assignable<typename std::add_lvalue_reference<T>::type, typename std::add_rvalue_reference<T>::type>
{};
template <class T>
inline constexpr bool is_move_assignable_v = is_move_assignable<T>::value;

template <class T>
struct is_trivially_move_assignable :
    std::is_trivially_assignable<typename std::add_lvalue_reference<T>::type,
                                 typename std::add_rvalue_reference<T>::type>
{};
template <class T>
inline constexpr bool is_trivially_move_assignable_v = is_trivially_move_assignable<T>::value;

template <class T>
struct is_nothrow_move_assignable :
    std::is_nothrow_assignable<typename std::add_lvalue_reference<T>::type, typename std::add_rvalue_reference<T>::type>
{};
template <class T>
inline constexpr bool is_nothrow_move_assignable_v = is_nothrow_move_assignable<T>::value;

// is_destructible
template <typename t>
    struct is_destructible : std::integral_constant < bool,
    requires(t object)
{
    object.~t();
} > {};
template <class T>
constexpr bool is_destructible_v = is_destructible<T>::value;

#if __has_builtin(__has_trivial_destructor)
// is_trivially_destructible
template <typename T>
struct is_trivially_destructible : conjunction<is_destructible<T>, bool_constant<__has_trivial_destructor(T)>>
{};
 template <class T>
 constexpr bool is_trivially_destructible_v = is_trivially_destructible<T>::value;
#else
#error is_trivially_destructible is not implemented
#endif // __has_builtin(__is_trivially_destructible)

template <typename t>
     struct is_nothrow_destructible : std::bool_constant<
    requires(t object)
{
    {
        object.~t()
    } noexcept;
} > {};
template <class T>
constexpr bool is_nothrow_destructible_v = is_nothrow_destructible<T>::value;

// has_virtual_destructor
template <class T>
struct has_virtual_destructor : std::bool_constant<__has_virtual_destructor(T)>
{};
template <class T>
inline constexpr bool has_virtual_destructor_v = has_virtual_destructor<T>::value;

// is_swappable
template <class T>
void swap(T& a, T& b) noexcept;

namespace detail {

struct __is_swappable
{
    template <class T, class U, class = decltype(swap(std::declval<T&>(), std::declval<U&>()))>
    static std::true_type __test(int);

    template <class, class>
    static std::false_type __test(...);
};

struct __is_nothrow_swappable
{
    template <class T, class U, class = decltype(nnoexcept(swap(std::declval<T&>(), std::declval<U&>())))>
    static std::true_type __test(int);

    template <class, class>
    static std::false_type __test(...);
};

}

template <class T>
struct is_swappable : decltype(detail::__is_swappable::__test<T, T>(0))
{};
template <class T>
inline constexpr bool is_swappable_v = is_swappable<T>::value;

// TODO implem: is_swappable_with
// template <class T, class U>
// struct is_swappable_with;
// template <class T, class U>
// inline constexpr bool is_swappable_with_v = is_swappable_with<T, U>::value;

template <class T>
struct is_nothrow_swappable : decltype(detail::__is_nothrow_swappable::__test<T, T>(0))
{};
template <class T>
inline constexpr bool is_nothrow_swappable_v = is_nothrow_swappable<T>::value;

// TODO implem: is_nothrow_swappable_with
// template <class T, class U> struct is_nothrow_swappable_with;
// template <class T, class U>
// inline constexpr bool is_nothrow_swappable_with_v = is_nothrow_swappable_with<T, U>::value;

// reference_converts_from_temporary
// TODO implem: reference_converts_from_temporary
// template <class T, class U>
// struct reference_converts_from_temporary;
// template <class T, class U>
// inline constexpr bool reference_converts_from_temporary_v =
//    std::reference_converts_from_temporary<T, U>::value;

// reference_constructs_from_temporary
// TODO implem: reference_constructs_from_temporary
// template <class T, class U>
// struct reference_constructs_from_temporary;
// template <class T, class U>
// inline constexpr bool reference_constructs_from_temporary_v =
//    std::reference_constructs_from_temporary<T, U>::value;
}
