/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef _STDBOOL_H_
#define _STDBOOL_H_

// 7.16 Boolean type and values <stdbool.h>

/* Don't define bool, true, and false in C++, except as a GNU extension. */
#ifndef __cplusplus

#define bool _Bool
#define true (1)
#define false (0)

#else /* __cplusplus */

#define _Bool bool
#define bool bool
#define false false
#define true true

#endif /* __cplusplus */

#define __bool_true_false_are_defined 1

#endif // _STDBOOL_H_
