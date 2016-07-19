#include <unistd.h>
#include <list>
#include <stdio.h>

class A {
	static std::list<int> test;
};

std::list<int> A::test;

int main(int argc,char* argv[]) {
	(void)argc;
	(void)argv;
	fputc('a', stdout);
	sleep(5);
	fputc('b', stdout);
	fputc('\n', stdout);
	return getpid();
}
