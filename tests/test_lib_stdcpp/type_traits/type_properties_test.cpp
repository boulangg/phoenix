/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #include <def/type_traits/type_properties.h>

#include <cstdint>
#include <string>

namespace libstdcpp {
namespace type_traits {

// is_const
namespace is_const {
static_assert(std::is_same_v<const int*, int const*> == true, "Remember, constness binds tightly inside pointers.");
static_assert(std::is_const_v<int> == false);
static_assert(std::is_const_v<const int> == true);
static_assert(std::is_const_v<int*> == false);
static_assert(std::is_const_v<int* const> == true,
              "Because the pointer itself can't be changed but the int pointed at can.");
static_assert(std::is_const_v<const int*> == false,
              "Because the pointer itself can be changed but not the int pointed at.");
static_assert(std::is_const_v<const int&> == false);
static_assert(std::is_const_v<std::remove_reference_t<const int&>> == true);

struct S
{
    void foo() const {}
    void bar() const {}
};

void test()
{
    // A const member function is const in a different way:

    static_assert(std::is_const_v<decltype(&S::foo)> == false, "Because &S::foo is a pointer.");

    using S_mem_fun_ptr = void (S::*)() const;

    S_mem_fun_ptr sfp = &S::foo;
    sfp = &S::bar; // OK, can be re-pointed
    static_assert(std::is_const_v<decltype(sfp)> == false,
                  "Because sfp is the same pointer type and thus can be re-pointed.");

    const S_mem_fun_ptr csfp = &S::foo;
    // csfp = &S::bar; // Error
    static_assert(std::is_const_v<decltype(csfp)> == true, "Because csfp cannot be re-pointed.");
}
}

// is_volatile
namespace is_volatile {
static_assert(std::is_volatile_v<int> == false);
static_assert(std::is_volatile_v<volatile int> == true);
static_assert(std::is_volatile_v<volatile const int> == true);
// TODO implem: valarray for is_volatile test
// static_assert(std::is_volatile_v<volatile std::valarray<float>> == true);
// static_assert(std::is_volatile_v<std::valarray<volatile float>> == false);
}

// is_trivial
namespace is_trivial {
struct A
{
    int m;
};
static_assert(std::is_trivial_v<A> == true);

struct B
{
    B() {}
};
static_assert(std::is_trivial_v<B> == false);
}

// is_trivially_copyable
namespace is_trivially_copyable {
struct A
{
    int m;
};
static_assert(std::is_trivially_copyable_v<A> == true);

struct B
{
    B(B const&) {}
};
static_assert(std::is_trivially_copyable_v<B> == false);

struct C
{
    virtual void foo();
};
static_assert(std::is_trivially_copyable_v<C> == false);

struct D
{
    int m;

    D(D const&) = default; // -> trivially copyable
    D(int x) : m(x + 1) {}
};
static_assert(std::is_trivially_copyable_v<D> == true);
}

// is_standard_layout
namespace is_standard_layout {
struct A
{
    int m;
};
static_assert(std::is_standard_layout_v<A> == true);

class B : public A
{
    int m;
};
static_assert(std::is_standard_layout_v<B> == false);

struct C
{
    virtual void foo();
};
static_assert(std::is_standard_layout_v<C> == false);
}

// is_pod (deprecated)
// TODO implem: is_pod for is_pod test
// namespace is_pod {
// struct A
//{
//    int m;
//};
// static_assert(std::is_pod_v<A> == true);
//
// class B : public A
//{
//    int m;
//};
// static_assert(std::is_pod_v<B> == false);
//
// struct C
//{
//    virtual void foo();
//};
// static_assert(std::is_pod_v<C> == false);
//}

// has_unique_object_representations
// TODO implem: has_unique_object_representations for has_unique_object_representations test
// namespace has_unique_object_representations {
// struct unpadded
//{
//    std::uint32_t a, b;
//};
//
// struct likely_padded
//{
//    std::uint8_t c;
//    std::uint16_t st;
//    std::uint32_t i;
//};
//
// int main()
//{
//    // Every value of a char corresponds to exactly one object representation.
//    static_assert(std::has_unique_object_representations_v<char> == true);
//    // For IEC 559 floats, assertion passes because the value NaN has
//    // multiple object representations.
//    static_assert(std::has_unique_object_representations_v<float> == false);
//
//    // Should succeed in any sane implementation because unpadded
//    // is typically not padded, and std::uint32_t cannot contain padding bits.
//    static_assert(std::has_unique_object_representations_v<unpadded> == true);
//    // Fails in most implementations because padding bits are inserted
//    // between the data members c and st for the purpose of aligning st to 16 bits.
//    static_assert(std::has_unique_object_representations_v<likely_padded> == false);
//
//    // Notable architectural divergence:
//    static_assert(std::has_unique_object_representations_v<bool> == true); // x86
//    // static_assert(!std::has_unique_object_representations_v<bool>); // ARM
//}
//}

// is_empty
namespace is_empty {
struct A
{};
static_assert(std::is_empty_v<A> == true);

struct B
{
    int m;
};
static_assert(std::is_empty_v<B> == false);

struct C
{
    static int m;
};
static_assert(std::is_empty_v<C> == true);

struct D
{
    virtual ~D();
};
static_assert(std::is_empty_v<D> == false);

union E
{};
static_assert(std::is_empty_v<E> == false);

struct F
{
    [[no_unique_address]] E e;
};
static_assert(std::is_empty_v<F> == true);

struct G
{
    int : 0;
    // C++ standard allow "as a special case, an unnamed bit-field with a width of zero
    // specifies alignment of the next bit-field at an allocation unit boundary.
    // Only when declaring an unnamed bit-field may the width be zero."
};
static_assert(std::is_empty_v<G> == true); // holds only unnamed bit-fields of zero width
}

// is_polymorphic
namespace is_polymorphic {
struct A
{
    int m;
};
static_assert(!std::is_polymorphic_v<A>);

struct B
{
    virtual void foo();
};
static_assert(std::is_polymorphic_v<B>);

struct C : B
{};
static_assert(std::is_polymorphic_v<C>);

struct D
{
    virtual ~D() = default;
};
static_assert(std::is_polymorphic_v<D>);

// Uses inheritance, but not the virtual keyword:
struct E : A
{};
static_assert(!std::is_polymorphic_v<E>);

struct F : virtual A
{};
static_assert(!std::is_polymorphic_v<F>);

struct AX : A
{};
struct AY : A
{};
struct XY : virtual AX, virtual AY
{};
static_assert(!std::is_polymorphic_v<XY>);
}

// is_abstract
namespace is_abstract {
struct A
{
    int m;
};
static_assert(std::is_abstract_v<A> == false);

struct B
{
    virtual void foo();
};
static_assert(std::is_abstract_v<B> == false);

struct C
{
    virtual void foo() = 0;
};
static_assert(std::is_abstract_v<C> == true);

struct D : C
{};
static_assert(std::is_abstract_v<D> == true);
}

// is_final
namespace is_final {
class A
{};
static_assert(std::is_final_v<A> == false);

class B final
{};
static_assert(std::is_final_v<B> == true);

union U final
{
    int x;
    double d;
};
static_assert(std::is_final_v<U> == true);
}

// is_aggregate
namespace is_aggregate {
struct A
{
    int x, y;
};
static_assert(std::is_aggregate_v<A>);

struct B
{
    int i;
    std::string str;

    B(int i, std::string str) : i(i), str(str) {}
};
static_assert(not std::is_aggregate_v<B> == true);
// TODO test: is_aggregate
//// Constructs a T at the uninitialized memory pointed to by p using
//// list-initialization for aggregates and non-list initialization otherwise.
// template <class T, class... Args>
// T* construct(T* p, Args&&... args)
//{
//     if constexpr (std::is_aggregate_v<T>)
//         return ::new (static_cast<void*>(p)) T{std::forward<Args>(args)...};
//     else
//         return ::new (static_cast<void*>(p)) T(std::forward<Args>(args)...);
// }
// template <typename... Ts>
// using aligned_storage_t = alignas(Ts...) std::byte[std::max({sizeof(Ts)...})];
//
// int main()
//{
//     aligned_storage_t<A, B> storage;
//
//     A& a = *construct(reinterpret_cast<A*>(&storage), 1, 2);
//     assert(a.x == 1 and a.y == 2);
//
//     B& b = *construct(reinterpret_cast<B*>(&storage), 3, "4");
//     assert(b.i == 3 and b.str == "4");
// }
}

// is_implicit_lifetime
// TODO implem: is_implicit_lifetime for is_implicit_lifetime test
// namespace is_implicit_lifetime {
// static_assert(std::is_implicit_lifetime_v<int> == true);       // arithmetic type is a scalar type
// static_assert(std::is_implicit_lifetime_v<const int> == true); // cv-qualified a scalar type
//
// enum E
//{
//    e
//};
// static_assert(std::is_implicit_lifetime_v<E> == true);              // enumeration type is a scalar type
// static_assert(std::is_implicit_lifetime_v<int*> == true);           // pointer type is a scalar type
// static_assert(std::is_implicit_lifetime_v<std::nullptr_t == true>); // scalar type
//
// struct S
//{
//    int x, y;
//};
//// S is an implicit-lifetime class: an aggregate without user-provided destructor
// static_assert(std::is_implicit_lifetime_v<S> == true);
//
// static_assert(std::is_implicit_lifetime_v<int S::*> == true); // pointer-to-member
//
// struct X
//{
//     ~X() = delete;
// };
//// X is not implicit-lifetime class due to deleted destructor
// static_assert(std::is_implicit_lifetime_v<X> == false);
//
// static_assert(std::is_implicit_lifetime_v<int[8]> == true);          // array type
// static_assert(std::is_implicit_lifetime_v<volatile int[8]> == true); // cv-qualified array type
// }

// is_signed
namespace is_signed {
class A
{};
static_assert(std::is_signed_v<A> == false);

class B
{
    int i;
};
static_assert(std::is_signed_v<B> == false);

enum C : int
{
};
static_assert(std::is_signed_v<C> == false);

enum class D : int
{
};
static_assert(std::is_signed_v<D> == false);

static_assert(std::is_signed<signed int>::value == true); // C++11
static_assert(std::is_signed<signed int>() == true);      // C++11
static_assert(std::is_signed<signed int>{} == true);      // C++11
static_assert(std::is_signed_v<signed int> == true);      // C++17
static_assert(std::is_signed_v<unsigned int> == false);
static_assert(std::is_signed_v<float> == true);
static_assert(std::is_signed_v<bool> == false);
static_assert(std::is_signed_v<signed char> == true);
static_assert(std::is_signed_v<unsigned char> == false);
#if __CHAR_UNSIGNED__
static_assert(std::is_signed_v<char> == false);
#else
static_assert(std::is_signed_v<char> == true);
#endif
}

// is_unsigned
namespace is_unsigned {
class A
{};
static_assert(std::is_unsigned_v<A> == false);

enum B : unsigned
{
};
static_assert(std::is_unsigned_v<B> == false);

enum class C : unsigned
{
};
static_assert(std::is_unsigned_v<C> == false);

struct S
{
    unsigned p : 1;
    int q      : 1;
};
static_assert(std::is_unsigned_v<decltype(S::p)> != std::is_unsigned_v<decltype(S::q)>);

static_assert(std::is_unsigned_v<float> == false);
static_assert(std::is_unsigned_v<signed int> == false);
static_assert(std::is_unsigned_v<unsigned int> == true);
static_assert(std::is_unsigned_v<bool> == true);
#if __CHAR_UNSIGNED__
static_assert(std::is_unsigned_v<char> == true);
#else
static_assert(std::is_unsigned_v<char> == false);
#endif
}

// is_bounded_array
namespace is_bounded_array {
class A
{};
static_assert(std::is_bounded_array_v<A> == false);
static_assert(std::is_bounded_array_v<A[]> == false);
static_assert(std::is_bounded_array_v<A[3]> == true);
static_assert(std::is_bounded_array_v<float> == false);
static_assert(std::is_bounded_array_v<int> == false);
static_assert(std::is_bounded_array_v<int[]> == false);
static_assert(std::is_bounded_array_v<int[3]> == true);
}

// is_unbounded_array
namespace is_unbounded_array {
class A
{};

static_assert(std::is_unbounded_array_v<A> == false);
static_assert(std::is_unbounded_array_v<A[]> == true);
static_assert(std::is_unbounded_array_v<A[3]> == false);
static_assert(std::is_unbounded_array_v<float> == false);
static_assert(std::is_unbounded_array_v<int> == false);
static_assert(std::is_unbounded_array_v<int[]> == true);
static_assert(std::is_unbounded_array_v<int[3]> == false);
}

// is_scoped_enum
namespace is_scoped_enum {
class A
{};

enum E
{
};

enum struct Es
{
    oz
};

enum class Ec : int
{
};

static_assert(std::is_scoped_enum_v<A> == false);
static_assert(std::is_scoped_enum_v<E> == false);
static_assert(std::is_scoped_enum_v<Es> == true);
static_assert(std::is_scoped_enum_v<Ec> == true);
static_assert(std::is_scoped_enum_v<int> == false);
}

}
}
