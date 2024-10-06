/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #include <def/type_traits/functions.h>

#include <def/type_traits/_is_same.h>

namespace libstdcpp {
namespace type_traits {

// is_pointer_interconvertible_with_class
// TODO implem: is_pointer_interconvertible_with_class for is_pointer_interconvertible_with_class test
//namespace is_pointer_interconvertible_with_class {
//struct Foo
//{
//    int x;
//};
//struct Bar
//{
//    int y;
//};
//
//struct Baz : Foo, Bar
//{}; // not standard-layout
//
//static_assert(not std::is_same_v<decltype(&Baz::x), int Baz::*>);
//static_assert(std::is_pointer_interconvertible_with_class(&Baz::x));
//static_assert(not std::is_pointer_interconvertible_with_class<Baz, int>(&Baz::x));
//}

// is_corresponding_member
// TODO implem: is_corresponding_member for is_corresponding_member test
//namespace is_corresponding_member {
//struct Foo
//{
//    int x;
//    double d;
//};
//
//struct Bar
//{
//    int y;
//    double z;
//};
//
//struct Baz : Foo, Bar
//{}; // not standard-layout
//
//static_assert(std::is_same_v<decltype(&Baz::x), int Foo::*> == true );
//static_assert(std::is_same_v<decltype(&Baz::y), int Bar::*> == true);
//static_assert(std::is_corresponding_member(&Foo::x, &Bar::y) == true);
//static_assert(std::is_corresponding_member(&Foo::d, &Bar::z) == true);
//static_assert(std::is_corresponding_member(&Baz::x, &Baz::y) == true);
//static_assert(std::is_corresponding_member<Baz, Baz, int, int>(&Baz::x, &Baz::y) == false);
//}

// is_constant_evaluated
// TODO test: is_constant_evaluated

// is_constant_evaluated
// TODO test: is_constant_evaluated

}
}
