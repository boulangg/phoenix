/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <cstdint>

namespace kernel::core {

namespace syscall {
void setupSyscall();
}

}

#ifdef __cplusplus
extern "C"
{
#endif

// C functions
std::uint64_t syscall64(std::uint64_t a, std::uint64_t b, std::uint64_t c, std::uint64_t d, std::uint64_t e,
                        std::uint64_t f, std::uint64_t num);
// ASM functions
extern void enable_syscall();
extern void load_syscall(std::uint64_t STAR, std::uint64_t LSTAR, std::uint64_t CSTAR, std::uint32_t SFMASK);
extern std::uint64_t syscall64_handler();

#ifdef __cplusplus
}
#endif
