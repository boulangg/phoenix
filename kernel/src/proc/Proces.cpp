/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "Process.h"

#include <bit>
#include <cstring>
#include <vector>

#include "Kernel.h"
#include "mem/Page.h"
#include "utils/Elf64File.h"
#include "utils/Utils.h"

namespace kernel::proc {

Process::Process(mem::PageTable pageTable, std::uint64_t entryPoint) : _memDesc(pageTable), _parent(nullptr)
{
    _fs = fs::VFS::getInitFsHandler();
    initKernelProcess(entryPoint);
}

Process::Process(Process* parent, mem::PageTable pageTable, std::uint64_t entryPoint) :
    _memDesc(pageTable), _parent(parent), _fs(parent->_fs)
{
    initKernelProcess(entryPoint);
}

Process::State Process::getState()
{
    return _state;
}

void Process::setState(Process::State state)
{
    _state = state;
}

void Process::initKernelProcess(std::uint64_t entryPoint)
{
    mem::Page* kernelStackpage = Kernel::memory.allocPages(9);
    _memDesc.setKernelStack(kernelStackpage->getPhysicalAddr());
    std::uint64_t* start = reinterpret_cast<std::uint64_t*>(kernelStackpage->getKernelAddr() + KERNEL_STACK_SIZE - 128);

    _cpuState.rsp = reinterpret_cast<std::uint64_t>(start);
    *(start++) = entryPoint;

    _cpuState.cr3 = _memDesc.getPageTablePtr();
}

void Process::initUserProcess(const char* argv[], const char* envp[], auxv_t auxv[])
{
    // Init kernel stack
    mem::Page* kernelStackpage = Kernel::memory.allocPages(9);
    _memDesc.setKernelStack(kernelStackpage->getPhysicalAddr());

    // Compute kernel stack min size
    std::size_t argc = 0;
    std::size_t argLen = 0;
    while (argv[argc] != nullptr) {
        argLen += strlen(argv[argc]) + 1;
        ++argc;
    }

    std::size_t envc = 0;
    std::size_t envLen = 0;
    while (envp[envc]) {
        envLen += strlen(envp[envc]) + 1;
        ++envc;
    }

    std::size_t auxc = 0;
    while (auxv[envc].a_type != AT_NULL) {
        ++auxc;
    }

    // switch_to_user_space, entry_point, argv, argv[], 0, envp[], 0, auxv[], 0
    std::size_t paramSize =
        (1ul + 1 + 1 + argc + 1 + envc + 1) * sizeof(std::uint64_t) + auxc * sizeof(auxc) + sizeof(std::uint64_t);

    // argv + envp strings
    std::size_t strTableSize = argLen + envLen;

    std::size_t offset = paramSize + strTableSize;
    std::size_t order = std::bit_width(utils::alignUp(offset, PAGE_SIZE));
    mem::Page* page = Kernel::memory.allocPages(order);

    loadUserStack();
    std::uint64_t userSpaceStart = _userStackTop - (1ul << order) * PAGE_SIZE;
    std::uint64_t userSpaceOffset = KERNEL_BASE_LINEAR_MAPPING - userSpaceStart;
    std::uint16_t flags = mem::pt_flag::FLAG_P | mem::pt_flag::FLAG_U | mem::pt_flag::FLAG_W;
    _memDesc.updatePageTable(flags, userSpaceStart, flags, false, page, order);

    // Write data into the stack
    std::uint64_t* start =
        reinterpret_cast<std::uint64_t*>(page->getKernelAddr() + (1ul << order) * PAGE_SIZE - offset);
    char* strTable = reinterpret_cast<char*>(start + paramSize);

    _cpuState.rsp = reinterpret_cast<std::uint64_t>(start);
    *(start++) = reinterpret_cast<std::uint64_t>(switch_to_user_space);
    *(start++) = _userEntryPoint;
    *(start++) = argc;

    for (std::size_t i = 0; i < argc; ++i) {
        *(start++) = reinterpret_cast<std::uint64_t>(strTable) - userSpaceOffset;
        strcpy(strTable, argv[i]);
        strTable += strlen(argv[i]);
    }

    for (std::size_t i = 0; i < envc; ++i) {
        *(start++) = reinterpret_cast<std::uint64_t>(strTable) - userSpaceOffset;
        strcpy(strTable, envp[i]);
        strTable += strlen(envp[i]);
    }

    for (std::size_t i = 0; i < auxc; ++i) {
        *(start++) = auxv[i].a_type;
        *(start++) = auxv[i].a_un.a_val;
    }
}

void Process::loadUserStack()
{
    std::uint32_t prot = mem::MemoryRegion::Prot::READ | mem::MemoryRegion::Prot::WRITE;
    std::uint32_t flags = mem::MemoryRegion::Flags::PRIVATE | mem::MemoryRegion::Flags::ANON;

    _userStackBottom = _memDesc.mmap(USER_STACK_BOTTOM, USER_STACK_SIZE, prot, flags, nullptr, 0);
    _userStackTop = _userStackBottom + USER_STACK_SIZE;
}
}
