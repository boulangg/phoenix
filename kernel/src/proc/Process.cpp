/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "Process.hpp"

#include <core/Console.hpp>
#include <driver/input/KeyboardDevice.hpp>
#include <fs/Elf64.hpp>
#include <utility>

#include <proc/ProcessScheduler.hpp>

Process* Process::scheduler = nullptr;

Process::Process(Process* parent, int pid, int flags) :
    pid(pid), prio(0), state(ProcessState::Ready), wakeUp(0), retval(0) /*, localOpenFileTable()*/
{
    (void)flags;
    ppid = parent->pid;
    pgid = parent->pgid;
    sid = parent->sid;
    name = parent->name;
    mapping = new VirtualMapping(*(parent->mapping));
    regSave[7] = parent->regSave[7];
    regSave[8] = mapping->getPageTable()->getPageTablePtr();
    procDir = new ProcDir(*(parent->procDir));
    save_regs(regSave);
    // tty = new TTYFile();
    // tty = parent->tty;
    fileDescriptorTable = parent->fileDescriptorTable;
}

Process::Process(int prio, code_type code) :
    prio(prio), state(ProcessState::Ready), wakeUp(0), retval(0) /*, localOpenFileTable()*/
{
    pid = 0;
    name = "idle";
    ppid = pid;
    pgid = pid;
    sid = pid;
    mapping = new VirtualMapping();
    mapping->setEntryPoint((uint64_t*)code);
    const char* tmp[] = {nullptr};
    mapping->initMainArgs(tmp, tmp);
    regSave[1] = (uint64_t) & (mapping->startStack[1]);
    regSave[7] = 0;
    regSave[8] = mapping->getPageTable()->getPageTablePtr();
    procDir = new ProcDir(VirtualFileSystem::root, VirtualFileSystem::root);
    // tty = new TTY();
    // Keyboard::setTTY(tty);
    open("/dev/tty", 0, 0);
    FileDescriptor fd = fileDescriptorTable.front();
    fileDescriptorTable.push_back(fd);
    fileDescriptorTable.push_back(fd);
}

int Process::execve(File* file, const char* argv[], const char* envp[])
{
    // if(fd < 0)
    //	return -1;
    if (file == nullptr) {
        return -1;
    }
    // TODO schedule delayed deletion to keep the kernelStack usable until next
    // context switch (or reuse the old one while keeping the kernel stacks)
    // delete mapping;
    mapping = Elf64::getVirtualMapping(file);
    if (mapping == nullptr)
        return -1;
    mapping->initMainArgs(argv, envp);
    regSave[1] = (uint64_t) & (mapping->startStack[0]);
    regSave[7] = RFLAGS_INIT;
    regSave[8] = mapping->getPageTable()->getPageTablePtr();
    return 0;
}

int Process::execve(code_type code, const char* argv[], const char* envp[])
{
    // TODO schedule delayed deletion to keep the kernelStack usable until next
    // context switch (or reuse the old one while keeping the kernel stacks)
    // delete mapping;
    mapping->setEntryPoint((uint64_t*)code);
    mapping->initMainArgs(argv, envp);
    regSave[1] = (uint64_t) & (mapping->startStack[1]);
    regSave[7] = 0;
    regSave[8] = mapping->getPageTable()->getPageTablePtr();
    return 0;
}

Process::~Process()
{
    // FIXME memory leak
    /*for (LocalOpenFile fd : localOpenFileTable) {
        ProcessScheduler::decrementGlobalFileRefCount(fd.openFileTableIndex);
    }*/
}

bool Process::operator<(const Process& p) const
{
    return prio < p.prio;
}

const std::string Process::getState(ProcessState state)
{
    switch (state) {
    case ProcessState::Running:
        return "Running";
    case ProcessState::Ready:
        return "Ready";
    case ProcessState::Dying:
        return "Dying";
    case ProcessState::Zombie:
        return "Zombie";
    case ProcessState::Sleeping:
        return "Sleeping";
    case ProcessState::SonBlocked:
        return "SonBlocked";
    case ProcessState::IOBlocked:
        return "IOBlocked";
    case ProcessState::SemaphoreBlocked:
        return "SemaphareBlocked";
    default:
        return "Unknown state !!";
    }
}
