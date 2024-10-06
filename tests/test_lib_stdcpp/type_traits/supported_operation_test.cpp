/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #include <def/type_traits/supported_operations.h>

#include <string>

namespace libstdcpp {
namespace type_traits {
// TODO test: type_traits supported_operations

// is_constructible
namespace is_constructible {
class Foo
{
    int v1;
    double v2;

public:
    Foo(int n) : v1(n), v2() {}
    Foo(int n, double f) noexcept : v1(n), v2(f) {}
};

static_assert(std::is_trivially_constructible_v<Foo, const Foo&> == true);
static_assert(std::is_trivially_constructible_v<Foo, int> == false);
static_assert(std::is_constructible_v<Foo, int> == true);
static_assert(std::is_nothrow_constructible_v<Foo, int> == false);
static_assert(std::is_nothrow_constructible_v<Foo, int, double> == true);
}

// is_default_constructible
namespace {
// TODO: implem is_trivially_default_constructible_v for is_default_constructible test
//struct S1
//{
//    std::string str; // member has a non-trivial default constructor
//};
//static_assert(std::is_default_constructible_v<S1> == true);
//static_assert(std::is_trivially_default_constructible_v<S1> == false);

struct S2
{
    int n;
    S2() = default; // trivial and non-throwing
};
static_assert(std::is_trivially_default_constructible_v<S2> == true);
static_assert(std::is_nothrow_default_constructible_v<S2> == true);
}

// is_copy_construtible
namespace is_copy_construtible {
struct S1
{
    std::string str; // member has a non-trivial copy constructor
};
static_assert(std::is_copy_constructible_v<S1> == true);
static_assert(std::is_trivially_copy_constructible_v<S1> == false);

struct S2
{
    int n;
    S2(const S2&) = default; // trivial and non-throwing
};
static_assert(std::is_trivially_copy_constructible_v<S2> == true);
static_assert(std::is_nothrow_copy_constructible_v<S2> == true);

struct S3
{
    S3(const S3&) = delete; // explicitly deleted
};
static_assert(std::is_copy_constructible_v<S3> == false);

struct S4
{
    S4(S4&){}; // can't bind const, hence not a copy-constructible
};
static_assert(std::is_copy_constructible_v<S4> == false);
}

// is_move_constructible
namespace is_move_constructible {
struct Ex1
{
    std::string str; // member has a non-trivial but non-throwing move constructor
};
static_assert(std::is_move_constructible_v<Ex1> == true);
static_assert(std::is_trivially_move_constructible_v<Ex1> == false);

// TODO implem: string nothrow constructor for is_move_constructible test
//static_assert(std::is_nothrow_move_constructible_v<Ex1> == true);

struct Ex2
{
    int n;
    Ex2(Ex2&&) = default; // trivial and non-throwing
};
static_assert(std::is_move_constructible_v<Ex2> == true);
static_assert(std::is_trivially_move_constructible_v<Ex2> == true);
static_assert(std::is_nothrow_move_constructible_v<Ex2> == true);

struct NoMove1
{
    // prevents implicit declaration of default move constructor;
    // however, the class is still move-constructible because its
    // copy constructor can bind to an rvalue argument
    NoMove1(const NoMove1&) {}
};
static_assert(std::is_move_constructible_v<NoMove1> == true);
static_assert(std::is_trivially_move_constructible_v<NoMove1> == false);
static_assert(std::is_nothrow_move_constructible_v<NoMove1> == false);

struct NoMove2
{
    // Not move-constructible since the lvalue reference
    // can't bind to the rvalue argument
    NoMove2(NoMove2&) {}
};
static_assert(std::is_move_constructible_v<NoMove2> == false);
static_assert(std::is_trivially_move_constructible_v<NoMove2> == false);
static_assert(std::is_nothrow_move_constructible_v<NoMove2> == false);
}

// is_assignable
namespace is_assignable {
struct Ex1
{
    int n;
};

static_assert(std::is_assignable_v<int, int> == false);
static_assert(std::is_assignable_v<int&, int> == true);
static_assert(std::is_assignable_v<int, double> == false);
static_assert(std::is_nothrow_assignable_v<int&, double> == true);
// TODO implem: assignment string from double
//static_assert(std::is_assignable_v<std::string, double> == true);
static_assert(std::is_trivially_assignable_v<Ex1&, const Ex1&> == true);
}

// is_copy_assignable
namespace is_copy_assignable {
struct Foo
{
    int n;
};

static_assert(std::is_trivially_copy_assignable_v<Foo> == true);
static_assert(std::is_copy_assignable_v<int[2]> == false);
static_assert(std::is_nothrow_copy_assignable_v<int> == true);
}

// is_move_assignable
namespace is_move_assignable {
struct Foo
{
    int n;
};

struct NoMove
{
    // prevents implicit declaration of default move assignment operator
    // however, the class is still move-assignable because its
    // copy assignment operator can bind to an rvalue argument
    NoMove& operator=(const NoMove&)
    {
        return *this;
    }
};

// TODO implem: string nothrow move-assignment for is_move_assignable test
// static_assert(std::is_nothrow_move_assignable_v<std::string> == true);
static_assert(std::is_move_assignable_v<int[2]> == false);
static_assert(std::is_trivially_move_assignable_v<Foo> == true);
static_assert(std::is_move_assignable_v<NoMove> == true);
static_assert(std::is_nothrow_move_assignable_v<NoMove> == false);
}

// is_destructible
namespace is_destructible {
struct Foo
{
    std::string str;
    ~Foo() noexcept {};
};

struct Bar
{
    ~Bar() = default;
};

static_assert(std::is_destructible_v<std::string> == true);
// TODO implem: is_trivially_destructible_v for is_destructible test
//static_assert(std::is_trivially_destructible_v<Foo> == false);
static_assert(std::is_nothrow_destructible_v<Foo> == true);
//static_assert(std::is_trivially_destructible_v<Bar> == true);
}

// has_virtual_destructor
namespace has_virtual_destructor {
struct S
{};
static_assert(std::has_virtual_destructor_v<S> == false);

struct B
{
    virtual ~B() {}
};
static_assert(std::has_virtual_destructor_v<B> == true);

struct D : B
{
    ~D() {}
};
static_assert(std::has_virtual_destructor_v<D> == true);
}

// is_swappable
// TODO test: is_swappable

// reference_converts_from_temporary
// TODO implem: reference_converts_from_temporary for reference_converts_from_temporary test
//namespace reference_converts_from_temporary {
//static_assert(std::reference_converts_from_temporary_v<int&&, int> == true);
//static_assert(std::reference_converts_from_temporary_v<const int&, int> == true);
//static_assert(std::reference_converts_from_temporary_v<int&&, int&&> == false);
//static_assert(std::reference_converts_from_temporary_v<const int&, int&&> == false);
//static_assert(std::reference_converts_from_temporary_v<int&&, long&&> == true);
//static_assert(std::reference_converts_from_temporary_v<int&&, long> == true);
//}

// reference_constructs_from_temporary
// TODO implem: reference_constructs_from_temporary for reference_constructs_from_temporary test
//namespace reference_constructs_from_temporary {
//static_assert(std::reference_constructs_from_temporary_v<int&&, int> == true);
//static_assert(std::reference_constructs_from_temporary_v<const int&, int> == true);
//static_assert(std::reference_constructs_from_temporary_v<int&&, int&&> == false);
//static_assert(std::reference_constructs_from_temporary_v<const int&, int&&> == false);
//static_assert(std::reference_constructs_from_temporary_v<int&&, long&&> == true);
//static_assert(std::reference_constructs_from_temporary_v<int&&, long> == true);
//}

}
}
