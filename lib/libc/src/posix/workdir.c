#include <unistd.h>

#include <syscall/syscall.h>

char *getcwd(char *buf, size_t size) {
	return sys_getcwd(buf, size);
}

int chdir(const char *path) {
	return sys_chdir(path);
}

int fchdir(int fd) {
	return sys_fchdir(fd);
}
