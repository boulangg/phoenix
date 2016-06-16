#include "ProcessGroup.hpp"

ProcessGroup::ProcessGroup(Process* leader) : gid(leader->getPid()), listProc() {
	listProc.push_back(leader);
}


void ProcessGroup::addProcess(Process* proc) {
	listProc.push_back(proc);
	proc->setpgid(gid);
}

ProcessGroup::~ProcessGroup() {

}

void ProcessGroup::removeProcess(Process* proc) {
	for (auto it = listProc.begin(); it != listProc.end(); ++it) {
		if (*it == proc) {
			listProc.erase(it);
			break;
		}
	}
}

bool ProcessGroup::empty() {
	return (listProc.size() == 0);
}
