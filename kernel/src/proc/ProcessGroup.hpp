#ifndef __PROC_PROCESSGROUP_HPP__
#define __PROC_PROCESSGROUP_HPP__

#include <list>

#include <proc/Process.hpp>

class ProcessGroup {
public:
	ProcessGroup(Process* leader);
	~ProcessGroup();

	void addProcess(Process* proc);
	void removeProcess(Process* proc);

	bool empty();

private:
	int gid;
	std::list<Process*> listProc;

};

#endif // __PROC_PROCESSGROUP_HPP__
