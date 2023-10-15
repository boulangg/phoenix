/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include <unistd.h>

#include <optional>
#include <string>
#include <type_traits>

class A
{
public:
    A(int i) : i(i) {}

    static int a;
    static A* b;

    int i;
};

int A::a = 0;
A* A::b = new A(3);

int a = 1;

class B
{
public:
    B() = default;

    B(int i) : i(i), str("test") {}

    int i;
    std::string str;
};

int main()
{
	std::optional<B> a1{2};
	std::optional<B> a2{};
	bool test = std::is_void<A>::value;
	(void)test;
	while (1) {

	}
	static int bss[8000];
	(void)bss;
	a = 0;
	return a;
}
