#include <new>
#include <cstdlib>

void *operator new(size_t size)
{
	if (size==0) {
		size = 1;
	}
    return malloc(size);
}

void *operator new[](size_t size)
{
	if (size==0) {
		size = 1;
	}
    return malloc(size);
}

void operator delete(void *p)
{
    free(p);
}

void operator delete[](void *p)
{
    free(p);
}
