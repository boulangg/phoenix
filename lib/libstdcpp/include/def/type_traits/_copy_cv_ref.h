/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <def/type_traits/references.h>

namespace std {
namespace details {
template <class From, class To>
struct __copy_cv
{
    using type = To;
};

template <class From, class To>
struct __copy_cv<const From, To>
{
    using type = const To;
};

template <class From, class To>
struct __copy_cv<volatile From, To>
{
    using type = volatile To;
};

template <class From, class To>
struct __copy_cv<const volatile From, To>
{
    using type = const volatile To;
};

template <class From, class To>
using __copy_cv_t = typename __copy_cv<From, To>::type;

template <class From, class To>
struct __copy_cvref
{
    using type = __copy_cv_t<From, To>;
};

template <class From, class To>
struct __copy_cvref<From&, To>
{
    using type = add_lvalue_reference_t<__copy_cv_t<From, To>>;
};

template <class From, class To>
struct __copy_cvref<From&&, To>
{
    using type = add_rvalue_reference_t<__copy_cv_t<From, To>>;
};

template <class From, class To>
using __copy_cvref_t = typename __copy_cvref<From, To>::type;
}
}
