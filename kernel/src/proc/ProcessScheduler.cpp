/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #include "ProcessScheduler.h"

namespace kernel::proc {

ProcessScheduler::ProcessScheduler() {}

void ProcessScheduler::init(mem::PageTable pgTable, void (*fn)())
{
    _currentProcess = new Process(pgTable, reinterpret_cast<std::uint64_t>(fn));

    load_new_task(_currentProcess->getCpuState());
}

void ProcessScheduler::schedule()
{
    Process* oldProcess = _currentProcess;
    _processes.push_back(_currentProcess);
    _currentProcess = _processes.front();
    _processes.pop_front();
    if (_currentProcess == oldProcess) {
        return;
    }

    context_switch(oldProcess->getCpuState(), _currentProcess->getCpuState());

}

}
