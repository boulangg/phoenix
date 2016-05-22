/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

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
