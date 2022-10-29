
#include <unistd.h>

#include <type_traits>
#include <optional>
#include <string>

class A
{
public:
	A(int i) : i(i)
	{}

	static int a;
	static A* b;

	int i;
};

int A::a = 0;
A* A::b = new A(3);

int a = 1;

class B {
public:
	B() = default;

	B(int i) : i(i), str("test")
	{}

	int i;
	std::string str;
};

int main()
{
	std::optional<B> a1{2};
	std::optional<B> a2{};
	bool test = std::is_void<A>::value;
	while (1) {

	}
	static int bss[8000];
	(void)bss;
	a = 0;
	return a;
}
