/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include <concepts>
#include <memory>
#include <utility>

namespace std {

class bad_function_call : public exception
{
public:
    bad_function_call() noexcept = default;
    bad_function_call(const bad_function_call& other) noexcept = default;

    virtual ~bad_function_call() = default;

    virtual const char* what() const noexcept
    {
        return "bad_function_call";
    }

    bad_function_call& operator=(const bad_function_call& other) noexcept = default;
};

template <class>
class callable_base;

template <class R, class... Args>
class callable_base<R(Args...)>
{
public:
    callable_base() = default;
    virtual ~callable_base() {}
    virtual R invoke(Args...) = 0;
    virtual std::unique_ptr<callable_base> clone() = 0;
};

template <class R, class... Args>
class callable_function : public callable_base<R(Args...)>
{
public:
    using function_pointer_type = R (*)(Args...);
    callable_function(const function_pointer_type& t) : _t(t) {}
    callable_function(function_pointer_type&& t) : _t(move(t)) {}

    R invoke(Args... args) override
    {
        return _t(std::forward<Args>(args)...);
    }

    std::unique_ptr<callable_base<R(Args...)>> clone() override
    {
        return std::make_unique<callable_function>(_t);
    }

private:
    function_pointer_type _t;
};

template <class R, class F, class... Args>
class callable_functor : public callable_base<R(Args...)>
{
public:
    using functor_pointer_type = F;
    callable_functor(const functor_pointer_type& f) : _fn(f) {}
    // callable_functor(functor_pointer_type&& t) : _t(move(t)) {}

    R invoke(Args... args) override
    {
        return _fn.operator()(std::forward<Args>(args)...);
    }

    std::unique_ptr<callable_base<R(Args...)>> clone() override
    {
        return std::make_unique<callable_functor>(_fn);
    }

private:
    functor_pointer_type _fn;
};

template <class R, class G, class... Args>
class callable_member : public callable_base<R(G, Args...)>
{
public:
    using member_pointer_type = R (G::*)(Args...);
    callable_member(const member_pointer_type& f) : _memFn(f) {}
    // callable_member(member_pointer_type&& t) : _t(move(t)) {}

    R invoke(G g, Args... args) override
    {
        return (g.*_memFn)(std::forward<Args>(args)...);
    }

    std::unique_ptr<callable_base<R(G, Args...)>> clone() override
    {
        return std::make_unique<callable_member>(_memFn);
    }

private:
    member_pointer_type _memFn;
};

namespace detail {
template <class R, class F, class... Args>
concept Functor = requires(F f, Args... args)
{
    {
        f.operator()(args...)
    }
    ->same_as<R>;
};

template <class R, class F, class G, class... Args>
concept MemberFunction = requires(F fn, G g, Args... args)
{
    {
        (g.*fn)(args...)
    }
    ->same_as<R>;
};
}

template <class>
class function;

template <class R, class... Args>
class function<R(Args...)>
{
public:
    using result_type = R;

    function() noexcept : _callable() {}
    function(std::nullptr_t) noexcept : _callable() {}

    function(R (*t)(Args...)) : _callable(new callable_function<R, Args...>(t)) {}

    template <class F>
    requires detail::Functor<R, F, Args...> function(F&& f) : _callable(new callable_functor<R, F, Args...>(f))
    {}

    template <class F>
    requires detail::Functor<R, F, Args...> function(const F&& f) : _callable(new callable_functor<R, F, Args...>(f))
    {}

    template <class G, class... Args2>
    requires detail::MemberFunction<R, R (G::*)(Args2...), G, Args2...> function(R (G::*f)(Args2...)) :
        _callable(new callable_member<R, G, Args2...>(f))
    {}

    template <class G, class... Args2>
    requires detail::MemberFunction<R, R (G::*)(Args2...), G, Args2...> function(const R (G::*f)(Args2...)) :
        _callable(new callable_member<R, G, Args2...>(f))
    {}

    function(const function& other) : _callable(other._callable ? other._callable->clone() : nullptr) {}

    function(function&& other) = default;

    function& operator=(const function& other)
    {
        _callable = other._callable->clone();
        return *this;
    }

    function& operator=(function&& other)
    {
        _callable.reset(other._callable);
        return *this;
    }

    function& operator=(std::nullptr_t) noexcept
    {
        _callable.release();
        return *this;
    }

    R operator()(Args... args)
    {
        if (_callable.get() == nullptr) {
            throw bad_function_call();
        }
        return _callable->invoke(std::forward<Args>(args)...);
    }

private:
    std::unique_ptr<callable_base<R(Args...)>> _callable;
};

}
