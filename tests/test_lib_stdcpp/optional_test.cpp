/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 
#include "UnitTestFramework.h"

#include <optional>
#include <string>

using namespace UnitTestFramework;

namespace libstdcpp {

TEST_CLASS(optional_test)
{
public:
    TEST_METHOD(constructor)
    {
        static_assert(std::is_convertible<int, int&&>::value);
        static_assert(std::is_convertible<int&&, int>::value);
        static_assert(std::is_copy_constructible<int>::value);
        static_assert(std::is_trivially_copy_constructible<int>::value);
        constexpr std::optional<int> o1{}; // empty
        constexpr std::optional<int> o2 = 1; // init from rvalue
        constexpr std::optional<int> o3{o2};  // copy-constructor

        static_assert(std::is_constructible_v<bool, const int&>);
        static_assert(std::is_convertible_v<const int&, bool>);
        static_assert(std::is_same_v<std::remove_cvref_t<bool>, bool>);
        constexpr std::optional<bool> b = std::optional<bool>{o1};
        static_assert(!b.has_value());
        static_assert(std::optional<bool>::__is_optional<std::optional<int>>::value);

        constexpr std::optional<bool> b2{false};
        constexpr std::optional<bool> b3{b2};
        static_assert(b2.has_value());
        static_assert(!b2.value());
        static_assert(b3.has_value());
        static_assert(!b3.value());
        //static_assert(std::is_trivially_move_constructible_v<std::optional<int>>);
        // TODO implem: string for optional test
        // std::optional<std::string> o4(std::in_place, {'a', 'b', 'c'});
        // std::optional<std::string> o5(std::in_place, 3, 'A');
        // std::optional o6(std::string{"deduction"});
    }

    TEST_METHOD(constructor_LWG3836)
    {
        constexpr std::optional<bool> b1{false};
        static_assert(b1.has_value());
        static_assert(!b1.value());

        constexpr std::optional<bool> b2{b1};
        static_assert(b2.has_value());
        static_assert(!b2.value());
    }
};

}
