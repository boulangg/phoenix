/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "ProcessScheduler.hpp"

#include <asm/cpu.h>

#include <core/Console.hpp>
#include <stdio.h>

#include <driver/PCI/IDE/IDEDriver.hpp>
#include <driver/ramdisk/RamDiskManager.hpp>
#include <errno.h>

extern "C" void PF_handler(int errorCode, void* addr)
{
    ProcessScheduler::pageFault(errorCode, addr);
}

int IOSetup()
{
    IDEDriver* ide = new IDEDriver();
    PCIManager::registerDriver(ide);

    PCIManager::initPCI();
    RamDiskManager::initRamDisk();

    VirtualFileSystem::mount("initrd", "/", "ext2", 0, nullptr);
    VirtualFileSystem::mount("apps", "/bin", "kernel", 0, nullptr);
    // VirtualFileSystem::mount("initrd", "/", "kernel", 0, nullptr);

    ProcessScheduler::exit(0);

    return -1;
}

int idle()
{
    if (ProcessScheduler::fork() == 0) {
        const char* init_argv[] = {"iosetup", nullptr};
        const char* init_envp[] = {nullptr};
        ProcessScheduler::execve(IOSetup, init_argv, init_envp);
    } else {
        sti();
        while (1) {
            hlt();
        }
        cli();
    }
    return 0;
}

uint64_t ProcessScheduler::nbProcess(0);
uint64_t ProcessScheduler::lastAssignedPid(-1);
Process* ProcessScheduler::running = nullptr;
std::vector<Process*> ProcessScheduler::processVector;
std::list<std::pair<Event, Process*>> ProcessScheduler::events;

std::priority_queue<Process*, std::vector<Process*>, ProcessLess> ProcessScheduler::ready;

// std::vector<GlobalOpenFile> ProcessScheduler::globalFileTable;

int ProcessScheduler::init()
{
    Process* proc;
    proc = Process::getIdleProc();
    proc->setpriority(IDLE_PRIO);
    nbProcess = 1;
    lastAssignedPid = 0;
    processVector.push_back(proc);
    running = proc;
    start_idle(proc->getRegSave());

    // TODO Unreachable code. Need to call this once IOSetup has been done!
    // Create init process
    std::string filename = "/bin/init";
    // File* f;
    struct ProcDir procDir = {VirtualFileSystem::root, VirtualFileSystem::root};
    int fd = VirtualFileSystem::open(&procDir, filename);
    // f = KernelFS::getUserApp(filename);
    // if(f==nullptr)
    //	return -1;
    if (fd < 0) {
        return -1;
    }
    File* file = VirtualFileSystem::filestable[fd];
    if (file == nullptr) {
        return -1;
    }

    int64_t pid = findPid();
    if (pid == -1)
        return -1;

    const char* init_argv[] = {"/bin/init", nullptr};
    const char* init_envp[] = {nullptr};

    proc = new Process(processVector[0], pid);
    proc->execve(file, init_argv, init_envp);
    proc->setName(filename);
    proc->setpriority(DEFAULT_PRIO);

    ready.push(proc);
    nbProcess++;
    lastAssignedPid = pid;
    // TODO might lead to inconsistency with pid
    processVector.push_back(proc);
    return pid;
}

void ProcessScheduler::schedule()
{
    // TODO delete dying

    Process* currProc = running;
    running->setState(ProcessState::Ready);

    ready.push(currProc);

    unconditionalContextSwitch(currProc);
}

void ProcessScheduler::exit(int status)
{
    Process* currProc = running;
    running->setState(ProcessState::Dying);
    cout << "pid: " << running->getPid() << ", retval: " << status << "\n";
    // TODO add to dying queue
    unconditionalContextSwitch(currProc);
}

pid_t ProcessScheduler::fork()
{
    int64_t pid = findPid();
    if (pid == -1)
        return -1;
    Process* newProc = new Process(running, pid);
    if (running->getPid() == pid) {
        // Child
        return 0;
    } else {
        nbProcess++;
        lastAssignedPid = pid;
        processVector.push_back(newProc);

        Process* currProc = running;
        running->setState(ProcessState::Ready);

        ready.push(currProc);

        newProc->setState(ProcessState::Running);
        running = newProc;
        ctx_sw(currProc->getRegSave(), newProc->getRegSave());
        return pid;
    }
}

int ProcessScheduler::execve(const char* filename, const char* argv[], const char* envp[])
{
    struct ProcDir* runningProcDir = running->getProcDir();
    int fd = VirtualFileSystem::open(runningProcDir, filename);
    if (fd < 0)
        return -1;
    File* file = VirtualFileSystem::filestable[fd];

    running->setName(std::string(filename));
    running->execve(file, argv, envp);

    load_new_task(running->getRegSave());

    return -1;
}

int ProcessScheduler::execve(Process::code_type code, const char* argv[], const char* envp[])
{
    running->setName(argv[0]);
    running->execve(code, argv, envp);

    load_new_task(running->getRegSave());

    return -1;
}

pid_t ProcessScheduler::getpid()
{
    return running->getPid();
}

void* ProcessScheduler::userBrk(void* addr)
{
    VirtualMapping* mapping = running->getMapping();

    return mapping->userBrk(addr);
}

int ProcessScheduler::pageFault(int errorCode, void* addr)
{
    VirtualMapping* mapping = running->getMapping();

    if (mapping->pageFault(errorCode, addr) != 0) {
        Console::write("Page Fault!!\n");
        exit(-1);
    }
    return 0;
}

int ProcessScheduler::open(const char* pathname, int flags, mode_t mode)
{
    return running->open(pathname, flags, mode);
}

char* ProcessScheduler::getcwd(char* buffer, size_t size)
{
    struct ProcDir* runningProcDir = running->getProcDir();
    std::string pathname = runningProcDir->workDir->getPathName();
    std::size_t pathSize = std::min(pathname.size(), size - 1);
    std::memcpy(buffer, pathname.c_str(), pathSize);
    buffer[pathSize] = '\0';
    return buffer;
}

int ProcessScheduler::chdir(const char* path)
{
    struct ProcDir* runningProcDir = running->getProcDir();
    std::string pathname(path);
    Dentry* dentry = VirtualFileSystem::getDentry(runningProcDir, pathname);
    // f = KernelFS::getUserApp(filename);
    if (dentry == nullptr)
        return -1;
    runningProcDir->workDir = dentry;
    // TODO free old Dentry workDir
    return 0;
}

int ProcessScheduler::fchdir(int fd)
{
    File* file = ProcessScheduler::getFile(fd);
    if (file == nullptr) {
        return ENOENT;
    }
    Dentry* dentry = file->getDentry();
    struct ProcDir* runningProcDir = running->getProcDir();
    runningProcDir->workDir = dentry;
    // TODO free old Dentry workDir
    return 0;
}

void ProcessScheduler::unconditionalContextSwitch(Process* currProc)
{
    Process* nextProc = ready.top();
    ready.pop();
    nextProc->setState(ProcessState::Running);
    running = nextProc;
    ctx_sw(currProc->getRegSave(), nextProc->getRegSave());
}

void ProcessScheduler::wakeUp(Event ev)
{
    // wake up all process waiting on the event
    // TODO remove pair(event, process) when a process wait on
    // multiple events
    for (auto it = events.begin(); it != events.end();) {
        auto& item = *it;
        if ((item.first == ev) && (item.second->getState() == ProcessState::EventBlocked)) {
            it = events.erase(it);
            item.second->setState(ProcessState::Ready);
            ready.push(item.second);
        } else {
            ++it;
        }
    }
}

void ProcessScheduler::wait(Event ev)
{
    // Set current process waiting on an event
    running->setState(ProcessState::EventBlocked);
    auto item = std::pair<Event, Process*>(ev, running);
    events.push_back(item);
    unconditionalContextSwitch(running);
}

/*void ProcessScheduler::sleep() {
    unconditionalContextSwitch(running);
}*/

File* ProcessScheduler::getFile(unsigned int fd)
{
    return running->getFile(fd);
}

int64_t ProcessScheduler::findPid()
{
    if (nbProcess == MAX_NB_PROCESS)
        return -1;

    unsigned int pid = (lastAssignedPid + 1) % MAX_NB_PROCESS;
    do {
        if (processVector[pid] == nullptr)
            return pid;

        pid = (pid + 1) % MAX_NB_PROCESS;
    } while (pid != lastAssignedPid);

    // On teste le dernier pid disponible
    if (processVector[pid] == nullptr)
        return pid;

    return -1;
}
