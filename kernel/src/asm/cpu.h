/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT
 * license
 * The license is available in the LICENSE file or at
 * https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include <cstdint>

#ifdef __cplusplus
extern "C"
{
#endif

inline static void outb(std::uint16_t port, std::uint8_t value)
{
    asm volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

inline static void outw(std::uint16_t port, std::uint16_t value)
{
    asm volatile("outw %0, %1" : : "a"(value), "Nd"(port));
}

inline static void outl(std::uint16_t port, std::uint32_t value)
{
    asm volatile("outl %0, %1" : : "a"(value), "Nd"(port));
}

inline static void outsw(std::uint16_t port, const void* buf, int size)
{
    asm volatile("cld; rep outsw" : "=S"(buf), "=c"(size) : "d"(port), "0"(buf), "1"(size) : "cc");
}

inline static void outsl(std::uint16_t port, const void* buf, int size)
{
    asm volatile("cld; rep outsl" : "=S"(buf), "=c"(size) : "d"(port), "0"(buf), "1"(size) : "cc");
}

inline static uint8_t inb(std::uint16_t port)
{
    uint8_t rega;
    asm volatile("inb %1,%0" : "=a"(rega) : "Nd"(port));
    return rega;
}

inline static uint16_t inw(std::uint16_t port)
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

inline static void insw(std::uint16_t port, void* buf, std::uint16_t size)
{
    asm volatile("cld; rep insw" : "=D"(buf), "=c"(size) : "d"(port), "0"(buf), "1"(size) : "memory", "cc");
}

inline static void insl(std::uint16_t port, void* buf, std::uint16_t size)
{
    asm volatile("cld; rep insl" : "=D"(buf), "=c"(size) : "d"(port), "0"(buf), "1"(size) : "memory", "cc");
}

// Enable interrupts
inline static void cli(void)
{
    asm volatile("cli" ::: "memory");
}

// Disable interrupts
inline static void sti(void)
{
    asm volatile("sti" ::: "memory");
}

inline static void hlt(void)
{
    asm volatile("hlt" ::: "memory");
}

inline static std::uint64_t rdtsc()
{
    std::uint32_t low, high;
    asm volatile("rdtsc" : "=a"(low), "=d"(high));
    return std::uint64_t(high) << 32 | low;
}

inline static void pause()
{
    asm volatile("pause" ::: "memory");
}

inline static std::uint64_t rflag()
{
    uint64_t rega;
    asm volatile("pushfq; popq %%rax;" : "=a"(rega));
    return rega;
}

inline static bool isInterruptEnabled()
{
    return (rflag() & (1<<9)) == 0;
}

// ASM functions
extern void enable_SSE();

#ifdef __cplusplus
}
#endif
