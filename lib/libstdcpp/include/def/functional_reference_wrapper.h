/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <def/memory_address_of.h>
#include <def/type_traits/_enable_if.h>
#include <def/type_traits/_is_same.h>
#include <def/type_traits/references.h>
#include <def/type_traits/_is_invocable.h>
#include <def/utility_declval.h>
#include <def/utility_forward.h>
#include <def/functional_invoke.h>

namespace std {

namespace detail {
template <class T>
constexpr T& FUN(T& t) noexcept
{
    return t;
}
template <class T>
void FUN(T&&) = delete;
}

template <class T>
class reference_wrapper
{
public:
    // types
    using type = T;

    // construct/copy/destroy
    template <class U, class = decltype(detail::FUN<T>(std::declval<U>()),
                                        std::enable_if_t<!std::is_same_v<reference_wrapper, std::remove_cvref_t<U>>>())>
    constexpr reference_wrapper(U&& u) noexcept(noexcept(detail::FUN<T>(std::forward<U>(u)))) :
        _ptr(std::addressof(detail::FUN<T>(std::forward<U>(u))))
    {}

    reference_wrapper(const reference_wrapper&) noexcept = default;

    // assignment
    reference_wrapper& operator=(const reference_wrapper& x) noexcept = default;

    // access
    constexpr operator T&() const noexcept
    {
        return *_ptr;
    }
    constexpr T& get() const noexcept
    {
        return *_ptr;
    }

    template <class... ArgTypes>
    constexpr std::invoke_result_t<T&, ArgTypes...> operator()(ArgTypes&&... args) const
        noexcept(std::is_nothrow_invocable_v<T&, ArgTypes...>)
    {
        return std::invoke(get(), std::forward<ArgTypes>(args)...);
    }

private:
    T* _ptr;
};

// deduction guides
template <class T>
reference_wrapper(T&) -> reference_wrapper<T>;
}
