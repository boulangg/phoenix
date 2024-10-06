/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

namespace std {
// Member relationships
// TODO implem: is_pointer_interconvertible_with_class
// template <class S, class M>
// constexpr bool is_pointer_interconvertible_with_class(M S::*mp) noexcept;

// TODO implem: is_corresponding_member
// template <class S1, class S2, class M1, class M2>
// constexpr bool is_corresponding_member(M1 S1::*mp, M2 S2::*mq) noexcept;

// Constant evaluation context
constexpr bool is_constant_evaluated() noexcept
{
    if consteval {
        return true;
    } else {
        return false;
    }
}

// TODO implem: is_within_lifetime
// template <class T>
// consteval bool is_within_lifetime(const T* ptr) noexcept;
}
