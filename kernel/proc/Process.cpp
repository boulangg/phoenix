/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "Process.hpp"

#include <core/Console.hpp>
#include <fs/Elf64.hpp>

#include <utility>

Process* Process::scheduler = nullptr;

Process::Process(Process* parent, int pid, int flags) :
		pid(pid), prio(0), state(ProcessState::Ready), wakeUp(0), retval(0) {
	(void)flags;
	ppid = parent->pid;
	pgid = parent->pgid;
	sid = parent->sid;
	name = parent->name;
	mapping = new VirtualMapping(*(parent->mapping));
	regSave[7] = parent->regSave[7];
	regSave[8] = mapping->getPageTable()->getPageTablePtr();
	save_regs(regSave);
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
	regSave[7] = 0;
	regSave[8] = mapping->getPageTable()->getPageTablePtr();
}

int Process::execve(File* f, const char* argv[], const char* envp[]) {
	if(f==nullptr)
		return -1;
	// TODO schedule delayed deletion to keep the kernelStack usable until next
	// context switch (or reuse the old one while keeping the kernel stacks)
	//delete mapping;
	mapping = Elf64::getVirtualMapping(f);
	if(mapping==nullptr)
		return -1;
	mapping->initMainArgs(argv, envp);
	regSave[1]= (uint64_t)&(mapping->startStack[0]);
	regSave[7] = RFLAGS_INIT;
	return 0;
}

Process::~Process() {
	// FIXME memory leak
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

