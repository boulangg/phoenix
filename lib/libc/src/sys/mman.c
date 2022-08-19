#include "sys/mman.h"

#include "syscall/syscall.h"

int mlock(const void* addr, size_t length)
{
	return sys_mlock(addr, length);
}

int mlockall(int flags)
{
	return sys_mlockall(flags);
}

int munlock(const void* addr, size_t length)
{
	return sys_munlock(addr, length);
}

int munlockall(void)
{
	return sys_munlockall();
}

void* mmap(void* addr, size_t length, int prot, int flags, int fd, off_t offset)
{
	return sys_mmap(addr, length, prot, flags, fd, offset);
}

int munmap(void* addr, size_t length)
{
	return sys_munmap(addr, length);
}

int mprotect(void* addr, size_t length, int prot)
{
	return sys_mprotect(addr, length, prot);
}

int msync(void* addr, size_t length, int flags)
{
	return sys_msync(addr, length, flags);
}

int shm_open(const char* name, int oflag, mode_t mode)
{
	// TODO not implemented
	(void)name;
	(void)oflag;
	(void)mode;
	return -1;
}

int shm_unlink(const char* name)
{
	// TODO not implemented
	(void)name;
	return -1;
}
