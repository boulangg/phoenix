/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "Process.hpp"

#include "ProcessTable.hpp"
#include <core/Console.hpp>
#include <fs/Elf64.hpp>

#include <utility>

Process* Process::scheduler = nullptr;

Process::Process(Process* parent, int pid, int flags) :
		pid(pid), prio(0), state(ProcessState::Ready), wakeUp(0), retval(0) {
	ppid = parent->pid;
	pgid = parent->pgid;
	sid = parent->sid;
	name = parent->name;
	for (int i = 0; i < 8; i++) {
		regSave[i] = parent->regSave[i];
	}
	mapping = parent->mapping;
	regSave[8] = mapping->getPageTable()->getPageTablePtr();
}

Process::Process(int prio, code_type code) :
		prio(prio), state(ProcessState::Ready), wakeUp(0), retval(0) {
	pid = 0;
	name = "idle";
	ppid = pid;
	pgid = pid;
	sid = pid;
	mapping = new VirtualMapping();
	mapping->setEntryPoint((uint64_t*)code);
	const char* tmp[] = {nullptr};
	mapping->initMainArgs(tmp,tmp);
	regSave[1]= (uint64_t)&(mapping->startStack[0]);
	regSave[8] = mapping->getPageTable()->getPageTablePtr();
}

int Process::execve(File* f, const char* argv[], const char* envp[]) {
	if(f==nullptr)
		return -1;
	delete mapping;
	mapping = Elf64::getVirtualMapping(f);
	if(mapping==nullptr)
		return -1;
	mapping->initMainArgs(argv,envp);
	regSave[1]= (uint64_t)&(mapping->startStack[0]);
	regSave[8] = mapping->getPageTable()->getPageTablePtr();
	return 0;
}

/*Process::Process(int pid,code_type code,std::string&& name,unsigned long ssize,int prio):
	pid(pid),name(std::move(name)),prio(prio),state(ProcessState::Ready),wakeUp(0),daddy(0),sonPid(0),
	retval(0),killed(false){
	mapping = new VirtualMapping();
	topStack = mapping->topStack;
	//topStack[-3]=(uint64_t)code;
	const char* tmp[] = {nullptr};
	mapping->initMainArgs(tmp,tmp);
	regSave[1]= (uint64_t)&(mapping->startCode[0]);
}*/

/*Process::Process(int pid,std::string&& name,VirtualMapping* mapping,unsigned long ssize,int prio,const char** argv, const char** envp):
		pid(pid),name(std::move(name)),prio(prio),state(ProcessState::Ready),topStack(mapping->topStack),wakeUp(0),daddy(0),sonPid(0),
	retval(0),killed(false),mapping(mapping){
	mapping->initMainArgs(argv,envp);
	regSave[1]= (uint64_t)&(mapping->startStack[0]);
}*/



Process::~Process() {
	//FIXME memory leak (but just for idle)
}

bool Process::operator<(const Process& p) const{
	return prio<p.prio;
}

const std::string Process::getState(ProcessState state) {
	switch(state){
	case ProcessState::Running :
		return "Running";
	case ProcessState::Ready :
		return "Ready";
	case ProcessState::Dying :
		return "Dying";
	case ProcessState::Zombie:
		return "Zombie";
	case ProcessState::Sleeping :
		return "Sleeping";
	case ProcessState::SonBlocked:
		return "SonBlocked";
	case ProcessState::IOBlocked:
		return "IOBlocked";
	case ProcessState::SemaphoreBlocked:
		return "SemaphareBlocked";
	default :
		return "Unknown state !!";
	}
}

