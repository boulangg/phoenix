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

/***************************************
 *               Defines               *
 ***************************************/
#define STACK_SIZE 512
#define MAXPRIO 256
#define MAX_NB_PROCESS 32

#ifdef __cplusplus
extern "C" {
#endif
extern void ctx_sw(uint64_t* curr, uint64_t* next,uint64_t pgTable);

#ifdef __cplusplus
}
#endif
enum class ProcessState{
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

class Process {
public:
	typedef int (*code_type) (int,char**,char**);
	typedef uint64_t size_type;
	typedef size_type* size_type_pointer;

	Process(int pid,code_type code,std::string&& name,unsigned long ssize,int prio);
	Process(int pid,std::string&& name,VirtualMapping* mapping,unsigned long ssize,int prio, const char* argv[], const char* envp[]);
	~Process();

	Process(int pid, int prio, const std::string& name, VirtualMapping* mapping);

	bool operator<(const Process& p) const;

	void setState(ProcessState s){state=s;}

	 VirtualMapping* getMapping() {return mapping;}

	size_type* getRegSave() {return regSave;}
private:
	static const std::string getState(ProcessState state);
	int pid;
	std::string name;
	int prio;
	ProcessState state;
	size_type regSave[9]; //	rbx, rsp, rbp, r12, r13, r14, r15, pg_dir,kernel_stack
	size_type_pointer topStack;
	size_type wakeUp;
	Process* daddy;
	int sonPid;
	int retval;
	bool killed;
	VirtualMapping* mapping;
};

#endif /* KERNEL_CORE_PROCESS_HPP_ */
