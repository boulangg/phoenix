/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include "Process.h"
#include "mem/PageTable.h"

namespace kernel::proc {

class ProcessScheduler
{
public:
    ProcessScheduler();
    void init(mem::PageTable pgTable, void (*fn)());
    void schedule();

private:
    std::list<Process*> _processes;
    Process* _currentProcess;
};

}

#ifdef __cplusplus
extern "C"
{
#endif

// ASM functions
extern void context_switch(kernel::proc::CpuState* curr, kernel::proc::CpuState* next);
extern void load_new_task(kernel::proc::CpuState* curr);

#ifdef __cplusplus
}
#endif
