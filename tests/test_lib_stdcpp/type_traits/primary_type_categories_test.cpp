/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #include <def/type_traits/type_categories.h>

namespace libstdcpp {
namespace type_traits {

void foo();
class A
{};

// is_void
namespace is_void {
static_assert(std::is_void_v<void> == true);
static_assert(std::is_void_v<const void> == true);
static_assert(std::is_void_v<volatile void> == true);
static_assert(std::is_void_v<void*> == false);
static_assert(std::is_void_v<int> == false);
static_assert(std::is_void_v<decltype(foo)> == false);
static_assert(std::is_void_v<std::is_void<void>> == false);
}

// is_null_pointer
namespace is_null_pointer {
static_assert(std::is_null_pointer_v<decltype(nullptr)>);
static_assert(!std::is_null_pointer_v<int*>);
static_assert(!std::is_pointer_v<decltype(nullptr)>);
static_assert(std::is_pointer_v<int*>);
}

// is_integral
namespace is_integral {
static_assert(std::is_integral_v<float> == false);
static_assert(std::is_integral_v<int*> == false);
static_assert(std::is_integral_v<int> == true);
static_assert(std::is_integral_v<const int> == true);
static_assert(std::is_integral_v<bool> == true);
static_assert(std::is_integral_v<char> == true);
static_assert(std::is_integral_v<A> == false);

struct B
{
    int x : 4;
};
static_assert(std::is_integral_v<B> == false);
using BF = decltype(B::x); // bit-field's type
static_assert(std::is_integral_v<BF> == true);

enum E : int
{};
static_assert(std::is_integral_v<E> == false);

template <class T>
constexpr T same(T i)
{
    static_assert(std::is_integral<T>::value == true, "Integral required.");
    return i;
}
static_assert(same('"') == 042);
}

// is_floating_point
namespace is_floating_point {
static_assert(!std::is_floating_point_v<A>);

static_assert(std::is_floating_point_v<float>);
static_assert(!std::is_floating_point_v<float&>);
static_assert(std::is_floating_point_v<double>);
static_assert(!std::is_floating_point_v<double&>);
static_assert(!std::is_floating_point_v<int>);
}

// is_array
namespace is_array {
static_assert(std::is_array<A>::value == false);
static_assert(std::is_array<A[]>::value == true);
static_assert(std::is_array<A[3]>::value == true);

static_assert(std::is_array<float>::value == false);
static_assert(std::is_array<int>::value == false);
static_assert(std::is_array<int[]>::value == true);
static_assert(std::is_array<int[3]>::value == true);
// TODO implem: array
// static_assert(std::is_array<std::array<int, 3>>::value == false);
}

// is_enum
namespace is_enum {
struct B
{
    enum E
    {};
};
static_assert(std::is_enum_v<B> == false);
static_assert(std::is_enum_v<B::E> == true);

enum C
{};
static_assert(std::is_enum_v<C> == true);

enum class D : int
{};
static_assert(std::is_enum_v<D> == true);

static_assert(std::is_enum_v<int> == false);
}

// is_union
namespace is_union {
static_assert(std::is_union_v<A> == false);

typedef union
{
    int a;
    float b;
} B;
static_assert(std::is_union_v<B> == true);

struct C
{
    B d;
};
static_assert(std::is_union_v<C> == false);

static_assert(std::is_union_v<int> == false);
}

// is_class
namespace is_class {
static_assert(std::is_class_v<A> == true);
static_assert(std::is_class_v<A*> == false);
static_assert(std::is_class_v<A&> == false);
static_assert(std::is_class_v<const A> == true);

enum class B
{};
static_assert(not std::is_class<B>::value == true);

union U
{
    class UC
    {};
};
static_assert(std::is_class_v<U> == false);
static_assert(std::is_class_v<U::UC> == true);

static_assert(std::is_class_v<int> == false);

static_assert(std::is_class_v<struct S> == true, "incomplete class");
static_assert(std::is_class_v<class C> == true, "incomplete class");
}

// is_function
namespace is_function {
static_assert(std::is_function_v<decltype(foo)> == true);

static_assert(std::is_function_v<int(int)> == true);
static_assert(std::is_function_v<int> == false);
static_assert(std::is_function_v<decltype([] {})> == false);
// TODO implem: std::function for is_function_v static_assert
// static_assert(!std::is_function_v<std::function<void()>>);

struct O
{
    void operator()() {}
};
static_assert(std::is_function_v<O()> == true);

struct B
{
    static int foo();
    int fun() const&;
};
static_assert(std::is_function_v<B> == false);
static_assert(std::is_function_v<decltype(B::foo)> == true);
static_assert(std::is_function_v<decltype(&B::fun)> == false);

template <typename>
struct PM_traits
{};
template <class T, class U>
struct PM_traits<U T::*>
{
    using member_type = U;
};

using T = PM_traits<decltype(&B::fun)>::member_type; // T is int() const&
static_assert(std::is_function_v<T> == true);
}

// is_pointer
namespace is_pointer {
struct B
{
    int m;
    void f() {}
};

int B::*mem_data_ptr = &B::m;     // a pointer to member data
void (B::*mem_fun_ptr)() = &B::f; // a pointer to member function

static_assert(std::is_pointer<A>::value == false);
static_assert(std::is_pointer_v<A> == false);                                // same thing as above, but in C++17!
static_assert(std::is_pointer<A>() == false); // same as above, using inherited operator bool
static_assert(std::is_pointer<A>{} == false); // ditto
static_assert(std::is_pointer<A>()() == false); // same as above, using inherited operator()
static_assert(std::is_pointer<A>{}() == false); // ditto
static_assert(std::is_pointer_v<A*> == true);
static_assert(std::is_pointer_v<A const* volatile> == true);
static_assert(std::is_pointer_v<A&> == false);
static_assert(std::is_pointer_v<decltype(mem_data_ptr)> == false);
static_assert(std::is_pointer_v<decltype(mem_fun_ptr)> == false);
static_assert(std::is_pointer_v<void*> == true);
static_assert(std::is_pointer_v<int> == false);
static_assert(std::is_pointer_v<int*> == true);
static_assert(std::is_pointer_v<int**> == true);
static_assert(std::is_pointer_v<int[10]> == false);
static_assert(std::is_pointer_v<std::nullptr_t> == false);
static_assert(std::is_pointer_v<void (*)()> == true);
}

// is_lvalue_reference
namespace is_lvalue_reference {
static_assert(std::is_lvalue_reference_v<A> == false);
static_assert(std::is_lvalue_reference_v<A&> == true);
static_assert(std::is_lvalue_reference_v<A&&> == false);

static_assert(std::is_lvalue_reference_v<int> == false);
static_assert(std::is_lvalue_reference_v<int&> == true);
static_assert(std::is_lvalue_reference_v<int&&> == false);
}

// is_rvalue_reference
namespace is_rvalue_reference {
static_assert(std::is_rvalue_reference_v<A> == false);
static_assert(std::is_rvalue_reference_v<A&> == false);
static_assert(std::is_rvalue_reference_v<A&&> != false);
static_assert(std::is_rvalue_reference_v<char> == false);
static_assert(std::is_rvalue_reference_v<char&> == false);
static_assert(std::is_rvalue_reference_v<char&&> != false);

template <typename T>
T&& test_rvalue(T&& x)
{
    static_assert(std::is_same_v<T&&, decltype(x)>);
    static_assert(std::is_rvalue_reference<T>::value == false);
    static_assert(std::is_rvalue_reference<T&&>::value == true);
    static_assert(std::is_rvalue_reference<decltype(x)>::value == true);
    return x;
}

template <typename T>
void test_lvalue(T&& x)
{
    static_assert(std::is_same_v<T&&, decltype(x)>);
    static_assert(std::is_rvalue_reference<T>::value == false);
    static_assert(std::is_rvalue_reference<T&&>::value == false);
    static_assert(std::is_rvalue_reference<decltype(x)>::value == false);
}

void test()
{
    test_rvalue(42);
    int x = 42;
    test_lvalue(x);
}
}

// is_member_object_pointer
namespace is_member_object_pointer {
static_assert(std::is_member_object_pointer_v<int(A::*)> == true);
static_assert(std::is_member_object_pointer_v<int(A::*)()> == false);
}

// is_member_function_pointer
namespace is_member_function_pointer {
class B
{
public:
    void member() {}
    int attribute;
};

static_assert(std::is_member_function_pointer<decltype(&B::member)>::value == true);
static_assert(std::is_member_function_pointer<decltype(&B::attribute)>::value == false);
}

}
}
