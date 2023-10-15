/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef NDEBUG
#define assert(ignore) ((void)0)
#else

#ifdef __cplusplus
extern "C" {
#endif

#define assert(expression)                                                                                             \
    if ((expression) == false) {                                                                                       \
        fprintf(stderr, "%s:%i %s: Assertion `%s' failed.\n", __FILE__, __LINE__, __FUNCTION__, #expression);          \
        exit(EXIT_FAILURE);                                                                                            \
    }

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // NDEBUG
