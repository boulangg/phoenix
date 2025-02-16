/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#define __bool_true_false_are_defined 1

#if __STDC_VERSION__ <= 202000L
#ifndef __cplusplus

#define true 1
#define false 0
#define bool char

#endif
#endif
