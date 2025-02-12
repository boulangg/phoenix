/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <def/type_traits/const_volatility_specifiers.h>
#include <def/type_traits/references.h>

namespace std {
template <class T>
struct remove_cvref
{
    using type = std::remove_cv_t<std::remove_reference_t<T>>;
};
template <class T>
using remove_cvref_t = remove_cvref<T>::type;
}
