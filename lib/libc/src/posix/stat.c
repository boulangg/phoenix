#include <sys/stat.h>

#include <syscall/syscall.h>

int stat(const char *pathname, struct stat* stat) {
	return sys_stat(pathname, stat);
}

int fstat(int fd, struct stat* stat) {
	return sys_fstat(fd, stat);
}

int lstat(const char *pathname, struct stat* stat) {
	return sys_lstat(pathname, stat);
}
