
#include <unistd.h>

#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <syscall/syscall.h>
#include <string.h>
#include <fcntl.h>

int main(int argc,char* argv[]) {
	(void)argc;
	(void)argv;
	if (fork() == 0) {
		char* const argv[3] = {(char*)"test1", (char*)"1", 0};
		char* const envp[1] = {0};
		execve("/bin/test1",argv, envp);
		return 10;
	} else {
		DIR* dir = opendir(".");
		struct dirent* dirp;
		while ((dirp = readdir(dir)) != NULL) {
			printf("%i: %s\n", dirp->d_ino, (char*)dirp->d_name);;
		}
		return getpid();
	}
}
