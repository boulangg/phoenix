/*
 * Copyright (c) 2016-2021 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include <new>
#include <cstdlib>

void *operator new(std::size_t size)
{
	if (size==0) {
		size = 1;
	}
    return malloc(size);
}

void * operator new (std::size_t size, void * v){
	return v;
}

void *operator new[](std::size_t size)
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
