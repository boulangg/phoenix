#include "ProcessScheduler.hpp"

#include <asm/cpu.h>
#include <fs/KernelFS.hpp>

#include <core/Console.hpp>
#include <stdio.h>

int idle()
{
	sti();
	while(1) {
		hlt();
	}
	cli();
	return 0;
}

uint64_t ProcessScheduler::nbProcess(0);
uint64_t ProcessScheduler::lastAssignedPid(-1);
Process* ProcessScheduler::running = nullptr;
std::vector<Process*> ProcessScheduler::processVector;

std::priority_queue<Process*,std::vector<Process*>,ProcessLess> ProcessScheduler::ready;

int ProcessScheduler::init(){
	Process* proc;
	proc = Process::getIdleProc();
	nbProcess = 1;
	lastAssignedPid=0;
	running=proc;
	processVector.push_back(proc);

	// Create init process
	std::string name = "init";
	File* f;
	f = KernelFS::getUserApp(name);
	if(f==nullptr)
		return -1;

	int64_t pid = findPid();
	if (pid == -1)
		return -1;

	const char* init_argv[] = {"init", nullptr};
	const char* init_envp[] = {nullptr};

	proc = new Process(processVector[0], pid);
	proc->execve(f, init_argv, init_envp);
	proc->setName(name);
	proc->getRegSave()[7] = RFLAGS_INIT;

	ready.push(proc);
	nbProcess++;
	lastAssignedPid=pid;
	processVector.push_back(proc);
	return pid;
}

void ProcessScheduler::schedule(){
	// TODO delete dying

	Process *currProc= running;
	running->setState(ProcessState::Ready);

	ready.push(currProc);

	unconditionalContextSwitch(currProc);
}

void ProcessScheduler::exit(int status) {
	Process *currProc= running;
	running->setState(ProcessState::Dying);
	char str[512];
	sprintf(str,"pid: %d, retval: %d\n", running->getPid(), status);
	Console::write(str);
	// TODO add to dying queue
	unconditionalContextSwitch(currProc);
}

pid_t ProcessScheduler::fork() {
	int64_t pid = findPid();
	if (pid == -1)
		return -1;
	Process* newProc = new Process(running, pid);
	if (running->getPid() == pid) {
		// Child
		return 0;
	} else {
		nbProcess++;
		lastAssignedPid=pid;
		processVector.push_back(newProc);

		Process *currProc= running;
		running->setState(ProcessState::Ready);

		ready.push(currProc);

		newProc->setState(ProcessState::Running);
		running = newProc;
		uint64_t pg_dir = newProc->getMapping()->getPageTable()->getPageTablePtr();
		ctx_sw(currProc->getRegSave(), newProc->getRegSave(), pg_dir);
		return pid;
	}
}

int ProcessScheduler::execve(const char* filename, const char* argv[], const char* envp[]) {
	File* f;
	f = KernelFS::getUserApp(filename);
	if(f==nullptr)
		return -1;

	running->setName(std::string(filename));
	running->execve(f, argv, envp);

	// load new execution context
	// this function should not return if everything goes well
	return -1;
}

pid_t ProcessScheduler::getpid() {
	return running->getPid();
}

void ProcessScheduler::unconditionalContextSwitch(Process* currProc) {
	Process *nextProc = ready.top();
	ready.pop();
	nextProc->setState(ProcessState::Running);
	running = nextProc;
	//PageTable::setActivePageTable(running->getMapping()->getPageTable());
	uint64_t pg_dir = nextProc->getMapping()->getPageTable()->getPageTablePtr();
	ctx_sw(currProc->getRegSave(), nextProc->getRegSave(),pg_dir);
}

int64_t ProcessScheduler::findPid() {

	if (nbProcess == MAX_NB_PROCESS)
		return -1;

	unsigned int pid = (lastAssignedPid+1) % MAX_NB_PROCESS;
	do{

		if (processVector[pid] == nullptr)
			return pid;

		pid = (pid+1) % MAX_NB_PROCESS;
	}while (pid != lastAssignedPid);

	// On teste le dernier pid disponible
	if (processVector[pid] == nullptr)
		return pid;

	return -1;
}
