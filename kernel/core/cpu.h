/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef KERNEL_CPU_H_
#define KERNEL_CPU_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

__inline__ static void cli(void)
{
	__asm__ __volatile__("cli":::"memory");
}

__inline__ static void sti(void)
{
	__asm__ __volatile__("sti":::"memory");
}

__inline__ static void hlt(void)
{
    __asm__ __volatile__ ("hlt":::"memory");
}

__inline__ static void outb(uint16_t port, uint8_t value)
{
	__asm__ __volatile__("outb %0, %1" : : "a" (value), "Nd" (port));
}

__inline__ static void outw(uint16_t port, uint16_t value)
{
	__asm__ __volatile__("outw %0, %1" : : "a" (value), "Nd" (port));
}

__inline__ static void outl(uint16_t port, uint32_t value)
{
	__asm__ __volatile__("outl %0, %1" : : "a" (value), "Nd" (port));
}

__inline__ static uint8_t inb(uint16_t port)
{
	uint8_t rega;
	__asm__ __volatile__("inb %1,%0" : "=a" (rega) : "Nd" (port));
	return rega;
}

__inline__ static uint16_t inw(uint16_t port)
{
	uint16_t rega;
	__asm__ __volatile__("inw %1,%0" : "=a" (rega) : "Nd" (port));
	return rega;
}

__inline__ static uint32_t inl(unsigned short port)
{
	uint32_t rega;
	__asm__ __volatile__("inl %1,%0" : "=a" (rega) : "Nd" (port));
	return rega;
}

#ifdef __cplusplus
}
#endif

#endif /* KERNEL_CPU_H_ */
