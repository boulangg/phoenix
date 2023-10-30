/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at
 * https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "ProcessScheduler.h"

namespace kernel::proc {

ProcessScheduler::ProcessScheduler() {}

void ProcessScheduler::init(mem::PageTable pgTable, void (*fn)())
{
    _current = new Process(pgTable, reinterpret_cast<std::uint64_t>(fn));
    _processes.push_back(_current);

    load_new_task(_current->getCpuState());
}

void ProcessScheduler::schedule()
{
    Process* oldProcess = _current;
    _runningProcesses.push_back(_current);
    _current = _runningProcesses.front();
    _runningProcesses.pop_front();
    if (_current == oldProcess) {
        return;
    }

    context_switch(oldProcess->getCpuState(), _current->getCpuState());
}

void ProcessScheduler::changeState(Process* proc, Process::State state)
{
    if (proc->getState() == state) {
        return;
    }

    switch (proc->getState()) {
    case Process::State::Running:
        _runningProcesses.remove(proc);
        break;
    case Process::State::Sleeping:
        _sleepingProcesses.remove(proc);
        break;
    default:
        break;
    }

    proc->setState(state);

    switch (proc->getState()) {
    case Process::State::Running:
        _runningProcesses.push_back(proc);
        break;
    case Process::State::Sleeping:
        _sleepingProcesses.push_back(proc);
        break;
    default:
        break;
    }
}

void ProcessScheduler::addProcess(Process* proc)
{
    _processes.push_back(proc);
    _runningProcesses.push_back(proc);
}

Process* ProcessScheduler::getCurrent()
{
    return _current;
}

}
