/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef _STRING_H_
#define _STRING_H_

// 7.21 String handling <string.h>
// 7.21.1 String function conventions
#include <stddef.h>
#include <def/def_restrict.h>

#ifdef __cplusplus
extern "C" {
#endif

// 7.21.2 Copying functions
void* memcpy(void* restrict dst, const void* restrict src, size_t count);

// Other
void* memset(void* dst, int ch, size_t count);

#ifdef __cplusplus
}
#endif

#endif /* _STRING_H_ */
