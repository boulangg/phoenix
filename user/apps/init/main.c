
#include <unistd.h>

int main(int argc,char* argv[]) {
	(void)argc;
	(void)argv;
	if (fork() == 0) {
		return 2;
	} else {
		return 4;
	}
}
