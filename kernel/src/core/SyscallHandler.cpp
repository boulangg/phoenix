/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #include "SyscallHandler.h"

#include "GlobalDescTable.h"

namespace kernel::core {

namespace syscall {
void setupSyscall()
{
    enable_syscall();
    std::uint64_t STAR =
        ((((uint64_t)GDT::USER_CS_64_OFFSET) << 48) | (((uint64_t)GDT::KERNEL_CS_64_OFFSET) << 32) | 0);
    load_syscall(STAR, (uint64_t)syscall64_handler, 0, 0);
}
}

}

#ifdef __cplusplus
extern "C"
{
#endif

std::uint64_t syscall64(std::uint64_t a, std::uint64_t b, std::uint64_t c, std::uint64_t d, std::uint64_t e,
                        std::uint64_t f, std::uint64_t num)
{
    (void)a;
    (void)b;
    (void)c;
    (void)d;
    (void)e;
    (void)f;
    (void)num;

    return 0;
}

#ifdef __cplusplus
}
#endif
