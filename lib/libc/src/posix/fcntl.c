#include <fcntl.h>

#include <stdarg.h>

#include <syscall/syscall.h>

int open(const char *pathname, int flags, ...) {
	va_list(va);
	va_start(va, flags);
	mode_t mode = va_arg(va, mode_t);
	va_end(va);
	return sys_open(pathname, flags, mode);
}
