
#include <unistd.h>

int main(int argc,char* argv[]) {
	(void)argc;
	(void)argv;
	if (fork() == 0) {
		char* const argv[3] = {"test1", "1", 0};
		char* const envp[1] = {0};
		execve("test1",argv, envp);
		return 10;
	} else {
		return getpid();
	}
}
