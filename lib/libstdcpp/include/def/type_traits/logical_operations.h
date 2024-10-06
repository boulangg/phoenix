/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <def/type_traits/_conditional.h>
#include <def/type_traits/integral_constant.h>

namespace std {
template <class...>
struct conjunction : std::true_type
{};
template <class B1>
struct conjunction<B1> : B1
{};
template <class B1, class... Bn>
struct conjunction<B1, Bn...> : std::conditional_t<bool(B1::value), conjunction<Bn...>, B1>
{};
template <class... B>
constexpr bool conjunction_v = conjunction<B...>::value;

template <class...>
struct disjunction : std::false_type
{};
template <class B1>
struct disjunction<B1> : B1
{};
template <class B1, class... Bn>
struct disjunction<B1, Bn...> : std::conditional_t<bool(B1::value), B1, disjunction<Bn...>>
{};
template <class... B>
constexpr bool disjunction_v = disjunction<B...>::value;

template <class B>
struct negation : std::bool_constant<!bool(B::value)>
{};
template <class B>
constexpr bool negation_v = negation<B>::value;
}
