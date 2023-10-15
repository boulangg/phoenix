/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef __PROC_PROCESSMANAGER_HPP__
#define __PROC_PROCESSMANAGER_HPP__

#include <unistd.h>
#include <proc/Process.hpp>
#include <queue>


extern "C" void ctx_sw(uint64_t * curr, uint64_t * next);
extern "C" void start_idle(uint64_t * idle);
extern "C" void load_new_task(uint64_t * curr);

#define IDLE_PRIO 0
#define DEFAULT_PRIO 20

class ProcessLess
{
public:
	bool operator()(const Process* lhs, const Process* rhs) const
	{
		return *lhs < *rhs;
	}
};

class Event
{
public:
	enum EventType
	{
		ErrorEvent,
		FileEvent,
		IOEvent,
		TimerEvent,
	};

	Event() : _evType(EventType::ErrorEvent), _id(0)
	{}
	Event(EventType evType, uint64_t id) : _evType(evType), _id(id)
	{}

	bool operator==(const Event& ev)
	{
		return (_evType == ev._evType && _id == ev._id);
	}

	uint64_t getId()
	{
		return _id;
	}

private:
	EventType _evType;
	uint64_t _id;
};

class ProcessScheduler
{
	ProcessScheduler() = delete;
	ProcessScheduler(const ProcessScheduler&) = delete;
	ProcessScheduler(ProcessScheduler&&) = delete;
	ProcessScheduler& operator=(const ProcessScheduler&) = delete;
	ProcessScheduler& operator=(ProcessScheduler&&) = delete;
	~ProcessScheduler() = delete;

public:
	// scheduler functions
	static int init();
	static void schedule();
	//static int start(const char *name, int prio,const char* argv[],const char* envp[]);

	// Syscalls
	static void exit(int status);
	static pid_t fork();
	//static void waitpid(pid_t pid, int* stat_adddr, int options);
	static int execve(const char* filename,
					  const char* argv[], const char* envp[]);
	static int execve(Process::code_type code,
					  const char* argv[], const char* envp[]);
	static pid_t getpid();
	static int pause();

	static Process* getCurrentProcess();
	static File* getFile(unsigned int fd);

	static int kill(pid_t pid, int sig);
	static int setpgid(pid_t pid, pid_t pgid);
	static pid_t getpgrid();
	static pid_t getpgrp();
	static pid_t setsid(); // Create new session if curr proc not leader
	static pid_t clone(unsigned long flags, void* child_stack,
					   void* ptid, void* ctid, struct pt_regs* regs);
	static pid_t getpgid(pid_t pid);
	static pid_t getsid(pid_t pid);
	static pid_t getuid();
	static pid_t getgid();

	static int open(const char* pathname, int flags, mode_t mode);
	static char* getcwd(char* buf, size_t size);
	static int chdir(const char* path);
	static int fchdir(int fd);

	// TODO solve race condition in wakeUp/wait/sleep
	static void wait(Event ev);
	static void wakeUp(Event ev);
	//static void sleep();

	static void* userBrk(void* addr);
	static int pageFault(int errorCode, void* addr);

	static void unconditionalContextSwitch(Process* currProc);

private:
	static int64_t findPid();

	static std::vector<Process*> processVector;
	static Process* running;
	static uint64_t nbProcess;
	static uint64_t lastAssignedPid;
	static std::priority_queue<Process*, std::vector<Process*>, ProcessLess> ready;
	static std::list<std::pair<Event, Process*>> events;

};


#endif // __PROC_PROCESSMANAGER_HPP__
