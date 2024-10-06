/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <def/type_traits/_decay.h>
#include <def/type_traits/integral_constant.h>
#include <def/type_traits/_is_base_of.h>
#include <def/type_traits/_enable_if.h>
#include <def/utility_declval.h>
#include <def/utility_forward.h>

namespace std {

// forward declaration
template <class T>
struct reference_wrapper;

/// result_of
namespace detail {
template <class T>
struct is_reference_wrapper : std::false_type
{};
template <class U>
struct is_reference_wrapper<std::reference_wrapper<U>> : std::true_type
{};

template <class T>
struct invoke_impl
{
    template <class F, class... Args>
    static auto call(F&& f, Args&&... args) -> decltype(std::forward<F>(f)(std::forward<Args>(args)...));
};

template <class B, class MT>
struct invoke_impl<MT B::*>
{
    template <class T, class Td = typename std::decay<T>::type,
              class = typename std::enable_if<std::is_base_of<B, Td>::value>::type>
    static auto get(T&& t) -> T&&;

    template <class T, class Td = typename std::decay<T>::type,
              class = typename std::enable_if<is_reference_wrapper<Td>::value>::type>
    static auto get(T&& t) -> decltype(t.get());

    template <class T, class Td = typename std::decay<T>::type,
              class = typename std::enable_if<!std::is_base_of<B, Td>::value>::type,
              class = typename std::enable_if<!is_reference_wrapper<Td>::value>::type>
    static auto get(T&& t) -> decltype(*std::forward<T>(t));

    template <class T, class... Args, class MT1, class = typename std::enable_if<std::is_function<MT1>::value>::type>
    static auto call(MT1 B::*pmf, T&& t, Args&&... args)
        -> decltype((invoke_impl::get(std::forward<T>(t)).*pmf)(std::forward<Args>(args)...));

    template <class T>
    static auto call(MT B::*pmd, T&& t) -> decltype(invoke_impl::get(std::forward<T>(t)).*pmd);
};

template <class F, class... Args, class Fd = typename std::decay<F>::type>
auto INVOKE(F&& f, Args&&... args) -> decltype(invoke_impl<Fd>::call(std::forward<F>(f), std::forward<Args>(args)...));

template <class R, class F, class... Args, class Fd = typename std::decay<F>::type,
          class std::enable_if<!std::is_void_v<R>, int>::type = 0>
auto INVOKE_R(F&& f, Args&&... args)
    -> decltype(R(invoke_impl<Fd>::call(std::forward<F>(f), std::forward<Args>(args)...)));

template <class R, class F, class... Args, class Fd = typename std::decay<F>::type,
          class std::enable_if<std::is_void_v<R>, int>::type = 0>
auto INVOKE_R(F&& f, Args&&... args)
    -> decltype(static_cast<void>(invoke_impl<Fd>::call(std::forward<F>(f), std::forward<Args>(args)...)));
} // namespace detail

// Conforming C++14 implementation (is also a valid C++11 implementation):
namespace detail {
template <typename AlwaysVoid, typename, typename...>
struct invoke_result
{};
template <typename F, typename... Args>
struct invoke_result<decltype(void(detail::INVOKE(std::declval<F>(), std::declval<Args>()...))), F, Args...>
{
    using type = decltype(detail::INVOKE(std::declval<F>(), std::declval<Args>()...));
};
} // namespace detail

namespace detail {
template <typename T, typename Enable = void>
struct is_invocable_impl : std::false_type
{};

template <typename F, typename... ArgTypes>
struct is_invocable_impl<F(ArgTypes...),
                         decltype(void(detail::INVOKE(std::declval<F>(), std::declval<ArgTypes>()...)))> :
    std::true_type
{};
}

template <class Fn, class... ArgTypes>
struct is_invocable : detail::is_invocable_impl<Fn && (ArgTypes && ...)>::type
{};
template <class Fn, class... ArgTypes>
inline constexpr bool is_invocable_v = is_invocable<Fn, ArgTypes...>::value;

namespace detail {
template <typename R, typename T, typename Enable = void>
struct is_invocable_r_impl : std::false_type
{};

template <typename R, typename F, typename... ArgTypes>
struct is_invocable_r_impl<F(ArgTypes...), R,
                           decltype(void(detail::INVOKE_R<R, F, ArgTypes...>(
                               std::declval<F>(), std::declval<ArgTypes>()...)))> : std::true_type
{};
}
template <class R, class Fn, class... ArgTypes>
struct is_invocable_r : detail::is_invocable_r_impl<Fn && (ArgTypes && ...), R>::type
{};
template <class R, class Fn, class... ArgTypes>
inline constexpr bool is_invocable_r_v = is_invocable_r<R, Fn, ArgTypes...>::value;

namespace detail {
template <typename T, typename Enable = void>
struct is_nothrow_invocable_impl : std::false_type
{};

template <typename F, typename... ArgTypes>
struct is_nothrow_invocable_impl<F(ArgTypes...),
                                 decltype(void(detail::INVOKE(std::declval<F>(), std::declval<ArgTypes>()...)))> :
    std::integral_constant<bool, noexcept(detail::INVOKE(std::declval<F>(), std::declval<ArgTypes>()...))>
{};
}

template <class Fn, class... ArgTypes>
struct is_nothrow_invocable : detail::is_nothrow_invocable_impl<Fn && (ArgTypes && ...)>::type
{};
template <class Fn, class... ArgTypes>
inline constexpr bool is_nothrow_invocable_v = is_nothrow_invocable<Fn, ArgTypes...>::value;

namespace detail {
template <typename R, typename T, typename Enable = void>
struct is_nothrow_invocable_r_impl : std::false_type
{};

template <typename R, typename F, typename... ArgTypes>
struct is_nothrow_invocable_r_impl<R, F(ArgTypes...),
                                   decltype(void(detail::INVOKE_R<R, F, ArgTypes...>(std::declval<F>(),
                                                                                     std::declval<ArgTypes>()...)))> :
    std::integral_constant<bool, noexcept(detail::INVOKE_R<R, F, ArgTypes...>(std::declval<F>(),
                                                                              std::declval<ArgTypes>()...))>
{};
}

template <class R, class Fn, class... ArgTypes>
struct is_nothrow_invocable_r;
template <class R, class Fn, class... ArgTypes>
inline constexpr bool is_nothrow_invocable_r_v = is_nothrow_invocable_r<R, Fn, ArgTypes...>::value;

template <class>
struct result_of;
template <class F, class... ArgTypes>
struct result_of<F(ArgTypes...)> : detail::invoke_result<void, F, ArgTypes...>
{};

template <class F, class... ArgTypes>
struct invoke_result : detail::invoke_result<void, F, ArgTypes...>
{};
template <class T>
using result_of_t = typename result_of<T>::type;
template <class F, class... ArgTypes>
using invoke_result_t = typename invoke_result<F, ArgTypes...>::type;
}
