#ifndef __PROC_PROCESSMANAGER_HPP__
#define __PROC_PROCESSMANAGER_HPP__

#include <proc/Process.hpp>
#include <queue>

extern "C" void ctx_sw(uint64_t* curr, uint64_t* next);
extern "C" void load_new_task(uint64_t* curr);

class ProcessLess {
public:
	bool operator()( const Process* lhs, const Process* rhs ) const {
		return *lhs < *rhs;
	}
};

typedef int pid_t;

class ProcessScheduler {
	ProcessScheduler() = delete;
	ProcessScheduler(const ProcessScheduler&) = delete;
	ProcessScheduler(ProcessScheduler&&) = delete;
	ProcessScheduler& operator=(const ProcessScheduler&) = delete;
	ProcessScheduler& operator=(ProcessScheduler &&) = delete;
	~ProcessScheduler() = delete;

public:
	// scheduler functions
	static int init();
	static void schedule();
	//static int start(const char *name, int prio,const char* argv[],const char* envp[]);

	// Syscalls
	static void exit(int status);
	static pid_t fork();
	static void waitpid(pid_t pid, int* stat_adddr, int options);
	static int execve(const char* filename,
			const char* argv[], const char* envp[]);
	static pid_t getpid();
	static int pause();
	static int kill(pid_t pid, int sig);
	static int setpgid(pid_t pid, pid_t pgid);
	static pid_t getpgrid();
	static pid_t getpgrp();
	static pid_t setsid(); // Create new session if curr proc not leader
	static pid_t clone(unsigned long flags, void* child_stack,
			void* ptid, void* ctid, struct pt_regs*regs);
	static pid_t getpgid(pid_t pid);
	static pid_t getsid(pid_t pid);
	static pid_t getuid();
	static pid_t getgid();

private:
	//static void initIdle();
	static int64_t findPid();
	static void unconditionalContextSwitch(Process* currProc);

	static std::vector<Process*> processVector;
	static Process* running;
	static uint64_t nbProcess;
	static uint64_t lastAssignedPid;
	static std::priority_queue<Process*,std::vector<Process*>,ProcessLess> ready;
};


#endif // __PROC_PROCESSMANAGER_HPP__
