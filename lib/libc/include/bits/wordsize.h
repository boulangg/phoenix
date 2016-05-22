/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef _KERNEL_BITS_WORDSIZE_H_
#define _KERNEL_BITS_WORDSIZE_H_

#if defined(__LP64__)
#define __WORDSIZE 64
#else
#define __WORDSIZE 32
#endif // __LP64__

#endif // _KERNEL_BITS_WORDSIZE_H_
