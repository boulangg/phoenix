/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef _MM_SBRK_H_
#define _MM_SBRK_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

	void* sys_brk(void* addr);

#ifdef __cplusplus
}
#endif

#endif // _MM_SBRK_H_
