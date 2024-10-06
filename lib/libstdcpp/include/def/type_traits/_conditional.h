/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

namespace std {
template <bool B, class T, class F>
struct conditional
{
    using type = T;
};
template <class T, class F>
struct conditional<false, T, F>
{
    using type = F;
};
template <bool B, class T, class F>
using conditional_t = typename conditional<B, T, F>::type;
}
