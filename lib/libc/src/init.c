/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "stdlib.h"
#include "stdio/io.h"
#include "sys/sys.h"

typedef void (*func_ptr)();
extern func_ptr __preinit_array_start[0], __preinit_array_end[0];
extern func_ptr __init_array_start[0], __init_array_end[0];
extern func_ptr __fini_array_start[0], __fini_array_end[0];

void _init() __attribute__((weak));
void _fini() __attribute__((weak));

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
}

void __libc_init(char* envp[])
{
	init_io();
	init_sys(envp);
}

void __libc_fini_array()
{
	size_t count;
	size_t i;

	count = __fini_array_end - __fini_array_start;
	for (i = count; i > 0; i--) {
		__fini_array_start[i - 1]();
	}

	_fini();

}

void __libc_start_main(int (*main) (int, char**, char**), int argc, char** argv, void (*init)(void), void (*fini)(void), void (*rtld_fini)(void), void(*stack_end))
{
	(void)init;
	(void)fini;
	(void)rtld_fini;
	(void)stack_end;
	__libc_init_array();
	atexit(__libc_fini_array);
	char** envp = argv + argc + 1;
	__libc_init(envp);
	exit(main(argc, argv, envp));
}
