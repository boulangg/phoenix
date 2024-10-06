/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <cstddef>

#include <def/type_traits/integral_constant.h>

namespace std {
template <class T>
struct alignment_of : std::integral_constant<std::size_t, alignof(T)>
{};
template <class T>
constexpr std::size_t alignment_of_v = alignment_of<T>::value;

template <class T>
struct rank : public std::integral_constant<std::size_t, 0>
{};
template <class T>
struct rank<T[]> : public std::integral_constant<std::size_t, rank<T>::value + 1>
{};
template <class T, std::size_t N>
struct rank<T[N]> : public std::integral_constant<std::size_t, rank<T>::value + 1>
{};
template <class T>
constexpr std::size_t rank_v = rank<T>::value;

template <class T, unsigned N = 0>
struct extent : std::integral_constant<std::size_t, 0>
{};
template <class T>
struct extent<T[], 0> : std::integral_constant<std::size_t, 0>
{};
template <class T, unsigned N>
struct extent<T[], N> : std::extent<T, N - 1>
{};
template <class T, std::size_t I>
struct extent<T[I], 0> : std::integral_constant<std::size_t, I>
{};
template <class T, std::size_t I, unsigned N>
struct extent<T[I], N> : std::extent<T, N - 1>
{};
template <class T, unsigned N = 0>
constexpr std::size_t extent_v = extent<T, N>::value;
}
