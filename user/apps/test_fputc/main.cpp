#include <unistd.h>
#include <list>
#include <stdio.h>
#include <string.h>

class A
{
	static std::list<int> test;
};

std::list<int> A::test;

int main(int argc, char* argv[])
{
	(void)argc;
	(void)argv;
	fputc('a', stdout);
	fputc('b', stdout);
	fputc('\n', stdout);

#define BUFFER_SIZE 16
	char buf[BUFFER_SIZE];
	memset(buf, '\0', BUFFER_SIZE);
	while (1) {
		ssize_t ret = read(1, buf, BUFFER_SIZE);
		for (int i = 0; i < ret; i++) {
			fputc(buf[i], stdout);
		}
	}

	return getpid();
}
