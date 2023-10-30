/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "Kernel.h"

#include <cstring>

#include "Constant.h"
#include "GlobalDescTable.h"
#include "asm/cpu.h"
#include "fs/ext2fs/Ext2SuperBlock.h"
#include "fs/kernelfs/KernelFile.h"
#include "fs/ramfs/RamSuperBlock.h"
#include "utils/Elf64File.h"

// Tests
#include "dev/input/InputManager.h"

namespace kernel {

mem::MemoryAllocator Kernel::memory;
core::InterruptDispatcher Kernel::interrupt;
proc::ProcessScheduler Kernel::scheduler;
core::clock::Clock* Kernel::clock;
dev::DeviceExplorer Kernel::deviceExplorer;
mem::MemoryDescriptor* Kernel::_kernelMemDesc;
console::BasicConsole Kernel::_console;
core::clock::RTCDevice* Kernel::_rtc;
core::clock::PITDevice* Kernel::_pit;

static void setupGlobalConstructors(utils::Elf64File& kernelFile)
{
    char sectionName[64];
    for (std::size_t i = 0; i < kernelFile.getFileHeader().e_shnum; ++i) {
        auto sHdr = kernelFile.getSectionHeader(i);
        kernelFile.getString(sHdr.sh_name, sectionName, 64);
        if (strcmp(".init_array", sectionName) == 0) {
            using func_ptr = void (*)(void);
            func_ptr* initArrayStart = reinterpret_cast<func_ptr*>(sHdr.sh_addr);
            std::size_t initArrayCount = sHdr.sh_size / sHdr.sh_entsize;

            for (std::size_t i = 0; i < initArrayCount; ++i) {
                initArrayStart[i]();
            }

            break;
        }
    }
}

static mem::MemoryDescriptor* initKernelPageTable(std::size_t hhdm, std::size_t pageCount, std::size_t kernelPhysBase,
                                                  utils::Elf64File& kernelFile)
{
    mem::PageTable table{};

    // Init level4 page table for kernel virtual address space
    for (auto i = 256; i < 512; ++i) {
        if (i == RESERVED_KERNEL_PAGE_TABLE_LVL4) {
            // Space for the kernel/syscall stack, not copied from parent
            continue;
        }
        mem::Page* lvl4Page = alloc_zeroed_page();
        table.initLevel4Page(i, lvl4Page->getPhysicalAddr());
    }

    std::uint16_t hhdmLvlFlags = mem::pt_flag::FLAG_P | mem::pt_flag::FLAG_W;

    // Set Higher Half Direct Mapping using 2M page size (512 * PAGE_SIZE)
    for (size_t i = 0; i < pageCount; i += 512) {
        std::uint64_t physAddr = i * PAGE_SIZE;
        std::uint64_t virtAddr = i * PAGE_SIZE + hhdm;
        table.mapPage(hhdmLvlFlags, virtAddr, hhdmLvlFlags, false, mem::page_size::pt_2MB, physAddr);
    }

    // Set Kernel Mapping
    for (std::size_t i = 0; i < kernelFile.getFileHeader().e_phnum; ++i) {
        auto pHdr = kernelFile.getProgramHeader(i);
        bool noExec = true;
        std::uint16_t flags = mem::pt_flag::FLAG_P;
        if (pHdr.p_type != kernel::utils::Elf64::ProgramType::PT_LOAD) {
            continue;
        }

        if (pHdr.p_flags & kernel::utils::Elf64::ProgramFlag::PF_W) {
            flags |= mem::pt_flag::FLAG_W;
        }
        if (pHdr.p_flags & kernel::utils::Elf64::ProgramFlag::PF_X) {
            noExec = false;
        }

        std::uint64_t physBase = kernel::utils::alignDown(kernelPhysBase + pHdr.p_offset, PAGE_SIZE);
        std::uint64_t virtBase = kernel::utils::alignDown(pHdr.p_vaddr, PAGE_SIZE);
        std::uint64_t virtTop = kernel::utils::alignUp(pHdr.p_vaddr + pHdr.p_memsz, PAGE_SIZE);
        std::size_t pageCount = (virtTop - virtBase) / PAGE_SIZE;

        for (std::size_t i = 0; i < pageCount; ++i) {
            std::size_t physAddr = physBase + i * PAGE_SIZE;
            std::size_t virtAddr = virtBase + i * PAGE_SIZE;
            table.mapPage(hhdmLvlFlags, virtAddr, flags, noExec, mem::page_size::pt_4KB, physAddr);
        }
    }

    // Set Base for Kernel Heap sbrk (2 MB)
    mem::Page* brkPage = alloc_zeroed_pages(9);
    std::uint64_t brkPagePhysAddr = brkPage->getPhysicalAddr();
    table.mapPage(hhdmLvlFlags, KERNEL_BRK_HEAP_START, hhdmLvlFlags, false, mem::page_size::pt_2MB, brkPagePhysAddr);

    set_CR3(table.getPageTablePhysAddr());

    mem::MemoryDescriptor* kernelMemDesc = new mem::MemoryDescriptor{table};
    kernelMemDesc->load(kernelFile, nullptr);

    return kernelMemDesc;
}

void Kernel::init(const KernelInfo& info)
{
    fs::kernelfs::KernelFile file(info.kernelFileAddr, info.kernelFileSize);
    utils::Elf64File kernelFile(&file);

    // Generic CPU struct
    GDT::setupGDT();
    TSS::setupTSS();

    // Memory Allocator
    memory.init(info.pageArray, info.pageCount);

    // Kernel base Page Table
    _kernelMemDesc = initKernelPageTable(info.hhdm, info.pageCount, info.kernelPhysBase, kernelFile);

    // Global constructors (requires initKernelPageTable for malloc)
    setupGlobalConstructors(kernelFile);

    // Logging
    _console = console::BasicConsole(info.framebuffer);

    // Interrupts
    interrupt.init();

    try {
        // Start the first Kernel thread
        printk("Start\n");
        scheduler.init(_kernelMemDesc->getPageTable(), Kernel::startIdleService);
        /// UNREACHABLE ///
    } catch (std::exception& exc) {
        printk("%s\n", exc.what());
    }
    printk("UNREACHABLE\n");
}

// Halt and catch fire function.
static void idle(void)
{
    sti();
    for (;;) {
        hlt();
        schedule();
    }
}

class TestHandler : public dev::input::InputHandler
{
public:
    void handleEvents(const std::vector<dev::input::InputEvent>& vals)
    {
        for (auto& val : vals) {
            if (val.type == EV_SYN)
                continue;
            printk("%.2x - %.2x - %.2x", val.type, val.code, val.value);
            printk(" (%.10u.%0.6u)\n", val.time.tv_sec, val.time.tv_nsec / 1000);
        }
    }
};

void Kernel::startIdleService()
{
    // Initialize Clock
    clock = new core::clock::Clock();
    _rtc = core::clock::RTCDevice::initRTC();
    _pit = core::clock::PITDevice::initPIT();

    sti();
    while (!clock->isClockInitialized()) {
        hlt();
    }
    cli();

    // Switch PIT to scheduler mode
    _pit->switchHandler();

    // Start main kernel process
    proc::Process* proc = new proc::Process(Kernel::scheduler.getCurrent(), _kernelMemDesc->getPageTable(),
                                            reinterpret_cast<std::uint64_t>(Kernel::startInitService));
    scheduler.addProcess(proc);

    // Go to idle
    idle();
}

void Kernel::startInitService()
{
    // Tests
    TestHandler handler;
    dev::input::InputManager::registerHandler(&handler);

    // Init block device service
    proc::Process* proc = new proc::Process(_kernelMemDesc->getPageTable(),
                                            reinterpret_cast<std::uint64_t>(Kernel::startBlockDeviceService));
    scheduler.addProcess(proc);

    // Init devices
    deviceExplorer.init();

    // Mount filesystem
    try {
        // Start the first Kernel thread
        printk("Start mounting\n");
        auto fs = Kernel::scheduler.getCurrent()->getFs();
        fs.root.dentry->getInode()->mkdir("root");
        fs.pwd = fs.root;
        fs::SuperBlock* sb = new fs::ext2fs::Ext2SuperBlock(deviceExplorer.getBlockDevice("sda"));
        fs::VFS::mount(fs.root, sb);
        printk("Finish mounting\n");
    } catch (std::invalid_argument& exc) {
        printk("Failed mounting\n");
        printk("%s\n", exc.what());
    }

    // Go to idle
    idle();
}

void Kernel::startBlockDeviceService()
{
    deviceExplorer.blockDeviceService();

    // Go to idle
    idle();
}

void Kernel::write(const char* str)
{
    _console.write(str);
}

timespec Kernel::getDateTime()
{
    return clock->getTime();
}

}
