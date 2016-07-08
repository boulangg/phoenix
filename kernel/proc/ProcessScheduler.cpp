/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "ProcessScheduler.hpp"

#include <asm/cpu.h>
#include <fs/KernelFS.hpp>

#include <core/Console.hpp>
#include <stdio.h>

extern "C" void PF_handler(int errorCode, void* addr) {
	ProcessScheduler::pageFault(errorCode, addr);
}

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
std::list<std::pair<Event, Process*>> ProcessScheduler::events;

std::priority_queue<Process*,std::vector<Process*>,ProcessLess> ProcessScheduler::ready;

int ProcessScheduler::init(){
	Process* proc;
	proc = Process::getIdleProc();
	proc->setpriority(IDLE_PRIO);
	nbProcess = 1;
	lastAssignedPid=0;
	running=proc;
	processVector.push_back(proc);
	ready.push(proc);
	unconditionalContextSwitch(proc); // Set page table to idle pagetable

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
	proc->setpriority(DEFAULT_PRIO);

	ready.push(proc);
	nbProcess++;
	lastAssignedPid=pid;
	// TODO might lead to inconsistency with pid
	processVector.push_back(proc);
	return pid;
}

void ProcessScheduler::schedule(){
	// TODO delete dying

	Process *currProc=running;
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
		ctx_sw(currProc->getRegSave(), newProc->getRegSave());
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

	load_new_task(running->getRegSave());

	return -1;
}

pid_t ProcessScheduler::getpid() {
	return running->getPid();
}

void* ProcessScheduler::userBrk(void* addr) {
	VirtualMapping* mapping = running->getMapping();

	return mapping->userBrk(addr);
}

int ProcessScheduler::pageFault(int errorCode, void* addr) {
	VirtualMapping* mapping = running->getMapping();

	return mapping->pageFault(errorCode, addr);
}

void ProcessScheduler::unconditionalContextSwitch(Process* currProc) {
	Process *nextProc = ready.top();
	ready.pop();
	nextProc->setState(ProcessState::Running);
	running = nextProc;
	ctx_sw(currProc->getRegSave(), nextProc->getRegSave());
}

void ProcessScheduler::wakeUp(Event ev) {
	// wake up all process waiting on the event
	// TODO remove pair(event, process) when a process wait on
	// multiple events
	for (auto it = events.begin(); it != events.end();) {
		auto item = *it;
		if ((item.first == ev) &&
		    (item.second->getState()==ProcessState::EventBlocked)) {
			it = events.erase(it);
			item.second->setState(ProcessState::Ready);
			ready.push(item.second);
		} else {
			++it;
		}
	}
}

void ProcessScheduler::wait(Event ev) {
	// Set current process waiting on an event
	running->setState(ProcessState::EventBlocked);
	auto item = std::pair<Event, Process*>(ev, running);
	events.push_back(item);
}

void ProcessScheduler::sleep() {
	unconditionalContextSwitch(running);
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