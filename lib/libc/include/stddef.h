/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef _KERNEL_STDDEF_H_
#define _KERNEL_STDDEF_H_

// 7.17 Common definitions <stddef.h>
#include "bits/wordsize.h"

typedef signed long int ptrdiff_t;

#include "def/def_size.h"

#include "def/def_null.h"

#define offsetof(type, member) __builtin_offsetof(type, member)

#endif // _KERNEL_STDDEF_H_
