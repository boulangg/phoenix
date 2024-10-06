/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

namespace std {
/// type_identity
template <class T>
struct type_identity
{
    using type = T;
};
template <class T>
using type_identity_t = typename type_identity<T>::type;
}
