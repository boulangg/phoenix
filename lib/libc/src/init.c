/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "stdio/io.h"

typedef void (*func_ptr)();
extern func_ptr __preinit_array_start[0], __preinit_array_end[0];
extern func_ptr __init_array_start[0], __init_array_end[0];
extern func_ptr __fini_array_start[0], __fini_array_end[0];

void _init() __attribute__((weak));
int main(int argc, char** argv) __attribute__((weak));
void _fini() __attribute__((weak));
void sys_exit(int) __attribute__((weak));

void __libc_init_array()
{
    size_t count;
    size_t i;

    count = __preinit_array_end - __preinit_array_start;
    for (i = 0; i < count; i++) {
        __preinit_array_start[i]();
    }

    _init();

    count = __init_array_end - __init_array_start;
    for (i = 0; i < count; i++) {
    	__init_array_start[i]();
    }

	init_io();
}


void __libc_fini_array()
{
    size_t count;
    size_t i;

    count = __fini_array_end - __fini_array_start;
    for (i = count; i > 0; i--) {
    	__fini_array_start[i-1]();
    }

    _fini();

}

void __libc_start_main(int argc, char** argv) {
	__libc_init_array();

    int ret = main(argc, argv);

    __libc_fini_array();

    sys_exit(ret);
}

