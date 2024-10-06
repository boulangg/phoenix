/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #include <def/type_traits/pointers.h>

#include <def/type_traits/type_categories.h>

namespace libstdcpp {
namespace type_traits {

// remove_pointer
namespace remove_pointer {
static_assert(std::is_same_v<int, int> == true);
static_assert(std::is_same_v<int, int*> == false);
static_assert(std::is_same_v<int, int**> == false);
static_assert(std::is_same_v<int, std::remove_pointer_t<int>> == true);
static_assert(std::is_same_v<int, std::remove_pointer_t<int*>> == true);
static_assert(std::is_same_v<int, std::remove_pointer_t<int**>> == false);
static_assert(std::is_same_v<int, std::remove_pointer_t<int* const>> == true);
static_assert(std::is_same_v<int, std::remove_pointer_t<int* volatile>> == true);
static_assert(std::is_same_v<int, std::remove_pointer_t<int* const volatile>> == true);
}

// add_pointer
namespace add_pointer {
template <typename F, typename Class>
void ptr_to_member_func_cvref_test(F Class::*)
{
    // F is an “abominable function type”
    using FF = std::add_pointer_t<F>;
    static_assert(std::is_same_v<F, FF>, "FF should be precisely F");
}

struct S
{
    void f_ref() & {}
    void f_const() const {}
};

void test()
{
    int i = 123;
    int& ri = i;
    typedef std::add_pointer<decltype(i)>::type IntPtr;
    typedef std::add_pointer<decltype(ri)>::type IntPtr2;

    static_assert(std::is_pointer_v<IntPtr>, "IntPtr should be a pointer");
    static_assert(std::is_same_v<IntPtr, int*>, "IntPtr should be a pointer to int");
    static_assert(std::is_same_v<IntPtr2, IntPtr>, "IntPtr2 should be equal to IntPtr");

    typedef std::remove_pointer<IntPtr>::type IntAgain;

    static_assert(!std::is_pointer_v<IntAgain>, "IntAgain should not be a pointer");
    static_assert(std::is_same_v<IntAgain, int>, "IntAgain should be equal to int");

    ptr_to_member_func_cvref_test(&S::f_ref);
    ptr_to_member_func_cvref_test(&S::f_const);
}
}

}
}
