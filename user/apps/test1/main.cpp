#include <unistd.h>
#include <list>

class A {
	static std::list<int> test;
};

std::list<int> A::test;

int main(int argc,char* argv[]) {
	(void)argc;
	(void)argv;
	sleep(5);
	return getpid();
}
