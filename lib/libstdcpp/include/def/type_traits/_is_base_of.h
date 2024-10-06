/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <def/type_traits/integral_constant.h>
#include <def/type_traits/type_categories.h>

namespace std {
// is_base_of
namespace details {
template <typename B>
std::true_type test_ptr_conv(const volatile B*);
template <typename>
std::false_type test_ptr_conv(const volatile void*);

template <typename B, typename D>
auto test_is_base_of(int) -> decltype(test_ptr_conv<B>(static_cast<D*>(nullptr)));
template <typename, typename>
auto test_is_base_of(...) -> std::true_type; // private or ambiguous base
}

template <typename Base, typename Derived>
struct is_base_of :
    std::integral_constant<bool, std::is_class<Base>::value &&
                                     std::is_class<Derived>::value&& decltype(details::test_is_base_of<Base, Derived>(
                                         0))::value>
{};
template <class Base, class Derived>
constexpr bool is_base_of_v = is_base_of<Base, Derived>::value;

// is_virtual_base_of
// TODO implem: is_virtual_base_of
// template <class Base, class Derived>
// struct is_virtual_base_of;
// template <class Base, class Derived>
// constexpr bool is_virtual_base_of_v = is_virtual_base_of<Base, Derived>::value;
}
