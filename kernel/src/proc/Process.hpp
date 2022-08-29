/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef KERNEL_CORE_PROCESS_HPP_
#define KERNEL_CORE_PROCESS_HPP_

#include <string>
#include "stdint.h"
#include <mm/VirtualMapping.hpp>
//#include <fs/File.hpp>
#include <fs/TTY.hpp>
#include <vector>
//#include <fs/vfs/VirtualFileSystem.hpp>

#include <fs/vfs/VirtualFileSystem.hpp>

/***************************************
 *               Defines               *
 ***************************************/
#define STACK_SIZE 512
#define MAXPRIO 256
#define MAX_NB_PROCESS 32

extern int idle();

extern "C" void save_regs(uint64_t * curr);

enum class ProcessState
{
	Running,
	Ready,
	SemaphoreBlocked,
	IOBlocked,
	EventBlocked,
	SonBlocked,
	Sleeping,
	Dying,
	Zombie
};

struct FileDescriptor
{
	int globalFileDescriptor;
};

class Process
{
public:
	typedef int (*code_type) ();
	typedef uint64_t size_type;
	typedef size_type* size_type_pointer;

	//Process(int pid,code_type code,std::string&& name,unsigned long ssize,int prio);
	//Process(int pid,std::string&& name,VirtualMapping* mapping,unsigned long ssize,int prio, const char* argv[], const char* envp[]);

	Process(Process* parent, int pid, int flags = 0);
	~Process();

	int execve(File* f, const char* argv[], const char* envp[]);
	int execve(code_type code, const char* argv[], const char* envp[]);

	bool operator<(const Process& p) const;

	void setState(ProcessState s)
	{
		state = s;
	}
	ProcessState getState()
	{
		return state;
	}

	VirtualMapping* getMapping()
	{
		return mapping;
	}

	size_type* getRegSave()
	{
		return regSave;
	}

	void setName(const std::string& name)
	{
		this->name = name;
	}

	int getPid()
	{
		return pid;
	}
	int setpgid(int pgid)
	{
		this->pgid = pgid; return 0;
	}

	int getpriority()
	{
		return prio;
	}
	int setpriority(int prio)
	{
		this->prio = prio; return 0;
	}

	static Process* getIdleProc()
	{
		if (scheduler == nullptr) {
			scheduler = new Process(0, idle);
		}
		return scheduler;
	}

	void switch_to_user_mode();

	File* getFile(unsigned int fd)
	{
/*if (fd >= 1 && fd <= 3) {
	return tty;
} else if (fd == 4) {
	File* f = VirtualFileSystem::filestable[0];
	return f;
} else {*/
		int gfd = fileDescriptorTable[fd].globalFileDescriptor;
		if (gfd < 0) {
			return nullptr;
		} else {
			return VirtualFileSystem::filestable[gfd];
		}
	//}
	}

	int open(const char* pathname, int flags, mode_t mode)
	{
		(void)flags; (void)mode;
		int gfd = VirtualFileSystem::open(procDir, pathname);
		if (gfd < 0) {
			return gfd;
		}
		for (size_t fd = 0; fd < fileDescriptorTable.size(); fd++) {
			if (fileDescriptorTable[fd].globalFileDescriptor < 0) {
				fileDescriptorTable[fd].globalFileDescriptor = gfd;
				return fd;
			}
		}
		FileDescriptor fdt_entry = { gfd };
		fileDescriptorTable.push_back(fdt_entry);
		return fileDescriptorTable.size() - 1;
	}

	struct ProcDir* getProcDir()
	{
		return procDir;
	}

private:
	// init idle
	Process(int prio, code_type code);

	static Process* scheduler;

	static const std::string getState(ProcessState state);

	int pid;
	int ppid;
	int pgid;
	int sid;
	std::string name;
	int prio;
	ProcessState state;
	size_type regSave[9]; //	rbx, rsp, rbp, r12, r13, r14, r15, rflags, pg_dir
	size_type wakeUp;
	int retval;
	VirtualMapping* mapping;
	//TTY* tty;
	struct ProcDir* procDir;

	std::vector<FileDescriptor> fileDescriptorTable;
};

#endif /* KERNEL_CORE_PROCESS_HPP_ */
