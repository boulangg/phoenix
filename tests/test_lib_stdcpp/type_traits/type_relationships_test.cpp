/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #include <def/type_traits/type_relationship.h>

#include <cstdint>

namespace libstdcpp {
namespace type_traits {

// is_same
namespace is_same {

// Depends on data model
static_assert(std::is_same_v<int, std::int32_t> == true);
static_assert(std::is_same_v<int, std::int64_t> == false);

// 'float' is never an integral type
static_assert(std::is_same_v<float, std::int32_t> == false);

// 'int' is implicitly 'signed'
static_assert(std::is_same_v<int, int> == true);
static_assert(std::is_same_v<int, unsigned int> == false);
static_assert(std::is_same_v<int, signed int> == true);

// unlike other types, 'char' is neither 'unsigned' nor 'signed'
static_assert(std::is_same_v<char, char> == true);
static_assert(std::is_same_v<char, unsigned char> == false);
static_assert(std::is_same_v<char, signed char> == false);

// const-qualified type T is not same as non-const T
static_assert(std::is_same<const int, int>() == false);
}

// is_base_of
namespace is_base_of {
class A
{};
class B : A
{};
class C : B
{};
class D
{};
union E
{};
using I = int;

static_assert(std::is_base_of_v<A, A> == true);
static_assert(std::is_base_of_v<A, B> == true);
static_assert(std::is_base_of_v<A, C> == true);
static_assert(std::is_base_of_v<A, D> == false);
static_assert(std::is_base_of_v<B, A> == false);
static_assert(std::is_base_of_v<E, E> == false);
static_assert(std::is_base_of_v<I, I> == false);
}

// is_virtual_base_of
// TODO implem: is_virtual_base_of for is_virtual_base_of test
// namespace is_virtual_base_of {
// class A
//{};
// class B : A
//{};
// class C : B
//{};
// class D : virtual A
//{};
// class E : D
//{};
//
// union F
//{};
// using I = int;
//
// static_assert(std::is_virtual_base_of_v<A, A> == false);
// static_assert(std::is_virtual_base_of_v<A, B> == false);
// static_assert(std::is_virtual_base_of_v<A, D> == true);
// static_assert(std::is_virtual_base_of_v<D, E> == false);
// static_assert(std::is_virtual_base_of_v<F, F> == false);
// static_assert(std::is_virtual_base_of_v<I, I> == false);
//}

// is_convertible
namespace is_convertible {

class A
{};
class B : public A
{};
class C
{};
class D
{
public:
    operator C()
    {
        return c;
    }
    C c;
};
class E
{
public:
    template <class T>
    E(T&&)
    {}
};

static_assert(std::is_convertible_v<B*, A*> == true);
static_assert(std::is_convertible_v<A*, B*> == false);
static_assert(std::is_convertible_v<D, C> == true);
static_assert(std::is_convertible_v<B*, C*> == false);
// Note that the Perfect Forwarding constructor makes the class E be
// "convertible" from everything. So, A is replaceable by B, C, D..:
static_assert(std::is_convertible_v<A, E> == true);

// TODO implem: string_view for is_convertible test
// static_assert(std::is_convertible_v<std::string_view, std::string> == false);
// static_assert(std::is_convertible_v<std::string, std::string_view> == true);
}

// is_layout_compatible
// TODO implem: is_layout_compatible for is_layout_compatible test
// namespace is_layout_compatible {
// struct Foo
//{
//    int x;
//    char y;
//};
//
// struct FooNua
//{
//    int x;
//    [[no_unique_address]] char y;
//};
//
// class Bar
//{
//    const int u = 42;
//    volatile char v = '*';
//};
//
// enum E0 : int
//{
//};
// enum class E1 : int
//{
//};
//
// static_assert(std::is_layout_compatible_v<const void, volatile void> == true);
// static_assert(std::is_layout_compatible_v<Foo, Bar> == true);
// static_assert(std::is_layout_compatible_v<Foo[2], Bar[2]> == false);
// static_assert(std::is_layout_compatible_v<int, E0> == false);
// static_assert(std::is_layout_compatible_v<E0, E1> == true);
// static_assert(std::is_layout_compatible_v<long, unsigned long> == false);
// static_assert(std::is_layout_compatible_v<char*, const char*> == false);
// static_assert(std::is_layout_compatible_v<char*, char* const> == true);
// static_assert(std::is_layout_compatible_v<Foo, FooNua> == false);
//}

// is_pointer_interconvertible_base_of
// TODO implem is_pointer_interconvertible_base_of for is_pointer_interconvertible_base_of test
// namespace is_pointer_interconvertible_base_of {
// struct Foo
//{};
//
// struct Bar
//{};
//
// class Baz : Foo, public Bar
//{
//    int x;
//};
//
// class NonStdLayout : public Baz
//{
//    int y;
//};
//
// static_assert(std::is_pointer_interconvertible_base_of_v<Bar, Baz> == true);
// static_assert(std::is_pointer_interconvertible_base_of_v<Foo, Baz> == true);
// static_assert(std::is_pointer_interconvertible_base_of_v<Baz, NonStdLayout> == false);
// static_assert(std::is_pointer_interconvertible_base_of_v<NonStdLayout, NonStdLayout> == true);
//}

// is_invocable
namespace is_invocable {
static_assert(std::is_invocable_v<int()> == true);
static_assert(std::is_invocable_v<int(), int> == false);

// TODO implem: is_invocable_r
// auto func2(char) -> int (*)()
//{
//     return nullptr;
// }
//static_assert(std::is_invocable_r_v<int, int()> == true);
//static_assert(std::is_invocable_r_v<int*, int()> == false);
//static_assert(std::is_invocable_r_v<void, void(int), int> == true);
//static_assert(std::is_invocable_r_v<void, void(int), void> == false);
//static_assert(std::is_invocable_r_v<int (*)(), decltype(func2), char> == true);
//static_assert(std::is_invocable_r_v<int (*)(), decltype(func2), void> == false);
}

}
}
