/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include <cstdint>

namespace kernel::core {

inline static void outb(uint16_t port, uint8_t value)
{
    asm volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

inline static void outw(uint16_t port, uint16_t value)
{
    asm volatile("outw %0, %1" : : "a"(value), "Nd"(port));
}

inline static void outl(uint16_t port, uint32_t value)
{
    asm volatile("outl %0, %1" : : "a"(value), "Nd"(port));
}

inline static void outsw(uint16_t port, const void* buf, int size)
{
    asm volatile("cld; rep outsw" : "=S"(buf), "=c"(size) : "d"(port), "0"(buf), "1"(size) : "cc");
}

inline static void outsl(uint16_t port, const void* buf, int size)
{
    asm volatile("cld; rep outsl" : "=S"(buf), "=c"(size) : "d"(port), "0"(buf), "1"(size) : "cc");
}

inline static uint8_t inb(uint16_t port)
{
    uint8_t rega;
    asm volatile("inb %1,%0" : "=a"(rega) : "Nd"(port));
    return rega;
}

inline static uint16_t inw(uint16_t port)
{
    uint16_t rega;
    asm volatile("inw %1,%0" : "=a"(rega) : "Nd"(port));
    return rega;
}

inline static uint32_t inl(unsigned short port)
{
    uint32_t rega;
    asm volatile("inl %1,%0" : "=a"(rega) : "Nd"(port));
    return rega;
}

inline static void insw(uint16_t port, void* buf, uint16_t size)
{
    asm volatile("cld; rep insw" : "=D"(buf), "=c"(size) : "d"(port), "0"(buf), "1"(size) : "memory", "cc");
}

inline static void insl(uint16_t port, void* buf, uint16_t size)
{
    asm volatile("cld; rep insl" : "=D"(buf), "=c"(size) : "d"(port), "0"(buf), "1"(size) : "memory", "cc");
}

inline static void cli(void)
{
    asm volatile("cli" ::: "memory");
}

inline static void sti(void)
{
    asm volatile("sti" ::: "memory");
}

inline static void hlt(void)
{
    asm volatile("hlt" ::: "memory");
}

}
