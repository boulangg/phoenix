#include <time.h>

#include <syscall/syscall.h>

int nanosleep(const struct timespec *req, struct timespec *rem) {
	return sys_nanosleep(req, rem);
}
