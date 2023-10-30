/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include <sys/auxv.h>

#include "mem/MemoryDescriptor.h"
#include "utils/Elf64File.h"

#include "ProcessFiles.h"

#include "fs/VFS.h"

namespace kernel::proc {

struct CpuState
{
    std::uint64_t rbx;
    std::uint64_t rsp;
    std::uint64_t rbp;
    std::uint64_t r12;
    std::uint64_t r13;
    std::uint64_t r14;
    std::uint64_t r15;
    std::uint64_t rflags;
    std::uint64_t cr3;
};

class Process
{
public:
    enum State
    {
        Running,
        Sleeping,
        Zombie,
        Dead
    };

    // Kernel process
    Process(mem::PageTable pageTable, std::uint64_t entryPoint);
    Process(Process* parent, mem::PageTable pageTable, std::uint64_t entryPoint);

    State getState();
    void setState(State state);

    CpuState* getCpuState()
    {
        return &_cpuState;
    }

    int execve(fs::File* file, const char* argv[], const char* envp[])
    {
        utils::Elf64File elf64File(0);
        _memDesc.load(elf64File, file);

        auto& hdr = elf64File.getFileHeader();
        _userEntryPoint = hdr.e_entry;

        auxv_t auxv[2] = {};
        auxv[0].a_type = AT_PAGESZ;
        auxv[0].a_un.a_val = PAGE_SIZE;
        auxv[1].a_type = AT_NULL;

        initUserProcess(argv, envp, auxv);

        _cpuState.rflags = 0;
        _cpuState.cr3 = _memDesc.getPageTablePtr();

        return 0;
    }

    void initKernelProcess(std::uint64_t entryPoint);
    void initUserProcess(const char* argv[], const char* envp[], auxv_t auxv[]);

    fs::FsHandler getFs() const
    {
        return _fs;
    }

private:
    void loadUserStack();

    State _state;
    mem::MemoryDescriptor _memDesc;
    ProcessFiles _procFile;
    CpuState _cpuState;
    std::uint64_t _userStackBottom;
    std::uint64_t _userStackTop;
    std::uint64_t _userEntryPoint;
    Process* _parent;
    fs::FsHandler _fs;
};
}

#ifdef __cplusplus
extern "C" {
#endif

// ASM functions
extern void switch_to_user_space();

#ifdef __cplusplus
}
#endif
