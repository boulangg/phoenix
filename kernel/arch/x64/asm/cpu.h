/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef __ARCH_CPU_H__
#define __ARCH_CPU_H__

#ifdef __cplusplus
extern "C" {
#endif

__inline__ static void cli(void) {
	__asm__ __volatile__("cli":::"memory");
}

__inline__ static void sti(void) {
	__asm__ __volatile__("sti":::"memory");
}

__inline__ static void hlt(void) {
	__asm__ __volatile__ ("hlt":::"memory");
}

#ifdef __cplusplus
}
#endif

#endif // __ARCH_CPU_H__
