/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <memory>
#include <utility>

namespace std {

template <typename T>
class function;

template <class R, class... Args>
class function<R(Args...)>
{
    // Base behavior types
    using invoke_fn_t = R(*)(char*, Args&&...);
    using construct_fn_t = void (*)(char*, char*);
    using destroy_fn_t = void (*)(char*);

    // Behavior implementation for generic functions
    template <typename Functor>
    static R invoke_fn(Functor* fn, Args&&... args)
    {
        return (*fn)(std::forward<Args>(args)...);
    }

    template <typename Functor>
    static void construct_fn(Functor* construct_dst, Functor* construct_src)
    {
        new (construct_dst) Functor(*construct_src);
    }

    template <typename Functor>
    static void destroy_fn(Functor* f)
    {
        f->~Functor();
    }

    // Storage behaviors
    invoke_fn_t invoke_f;
    construct_fn_t construct_f;
    destroy_fn_t destroy_f;

    // function_ptr
    char* data_ptr; // TODO use a unique_ptr
    size_t data_size;

public:
    function() : invoke_f(nullptr), construct_f(nullptr), destroy_f(nullptr), data_ptr(nullptr), data_size(0) {}

    // construct from any functor type
    template <typename Functor>
    function(Functor f) :
        invoke_f(reinterpret_cast<invoke_fn_t>(invoke_fn<Functor>)),
        construct_f(reinterpret_cast<construct_fn_t>(construct_fn<Functor>)),
        destroy_f(reinterpret_cast<destroy_fn_t>(destroy_fn<Functor>)), data_ptr(new char[sizeof(Functor)]),
        data_size(sizeof(Functor))
    {
        // copy the functor to internal storage
        this->construct_f(this->data_ptr, reinterpret_cast<char*>(&f));
    }

    // TODO other constructors, from nullptr, from function pointers

    // copy constructor
    function(function const& rhs) :
        invoke_f(rhs.invoke_f), construct_f(rhs.construct_f), destroy_f(rhs.destroy_f), data_size(rhs.data_size)
    {
        if (this->invoke_f) {
            // when the source is not a null function, copy its internal functor
            if (this->data_ptr != nullptr) {
                delete this->data_ptr;
            }
            this->data_ptr = new char[this->data_size];
            this->construct_f(this->data_ptr, rhs.data_ptr);
        }
    }

    ~function()
    {
        if (data_ptr != nullptr) {
            this->destroy_f(this->data_ptr);
            delete data_ptr;
        }
    }

    R operator()(Args&&... args)
    {
        return this->invoke_f(this->data_ptr, std::forward<Args>(args)...);
    }
};

}
