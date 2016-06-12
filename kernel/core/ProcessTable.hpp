/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef KERNEL_CORE_PROCESSTABLE_HPP_
#define KERNEL_CORE_PROCESSTABLE_HPP_

#include <vector>
#include <queue>
#include "Process.hpp"

int idle(int,char**,char**);

class ProcessLess{
public:
	bool operator()( const Process* lhs, const Process* rhs ) const {
		return *lhs < *rhs;
	}
};

class ProcessTable {
public:

	ProcessTable(const ProcessTable&) = delete;
	ProcessTable(ProcessTable&&) = delete;
	ProcessTable& operator=(const ProcessTable&) = delete;
	ProcessTable& operator=(ProcessTable &&) = delete;
	ProcessTable() = delete;
	~ProcessTable() = delete;
	static void init();
	static int start(const char *name, unsigned long ssize, int prio,const char* argv[],const char* envp[]);
	static void schedule();
	static void unconditionalContextSwitch(Process* currProc);
	static void do_exit(int retval);
private:
	static std::vector<Process*> processVector;
	static Process* running;
	static uint64_t nbProcess;
	static uint64_t lastAssignedPid;
	static void initIdle();
	static int64_t findPid();
	static std::priority_queue<Process*,std::vector<Process*>,ProcessLess> ready;
};

#endif /* KERNEL_CORE_PROCESSTABLE_HPP_ */
