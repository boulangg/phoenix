/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "stdio/io.h"

typedef void (*func_ptr)(void);
extern func_ptr init_array_start[0], init_array_end[0];

static void setupGlobalConstructors() {
	for (func_ptr* fn = init_array_start; fn != init_array_end; fn++) {
		(*fn)();
	}
}

void init_default_std() {
	setupGlobalConstructors();
	init_io();
}

