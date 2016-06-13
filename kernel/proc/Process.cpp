/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "Process.hpp"

#include "ProcessTable.hpp"
#include <core/Console.hpp>

#include <utility>

Process::Process(int pid, int prio, const std::string& name, VirtualMapping* mapping) :
		pid(pid),name(name),prio(prio),state(ProcessState::Ready),topStack(mapping->topStack),wakeUp(0),daddy(0),sonPid(0),
	retval(0),killed(false),mapping(mapping) {
	// TODO save mapping, remove topStack variable (and use startStack directly from mapping)
	// remove kernelStack: fix address mapped to different physical page for different process
	// don't need to init stack: done by initMainArgs(argv, envp)
	// require call to initMainArgs(argv, envp) before first context switch
	// require setKernelPageTable call before every context switch
}

Process::Process(int pid,code_type code,std::string&& name,unsigned long ssize,int prio):
	pid(pid),name(std::move(name)),prio(prio),state(ProcessState::Ready),wakeUp(0),daddy(0),sonPid(0),
	retval(0),killed(false){
	mapping = new VirtualMapping();
	topStack = mapping->topStack;
	//topStack[-3]=(uint64_t)code;
	const char* tmp[] = {nullptr};
	mapping->initMainArgs(tmp,tmp);
	regSave[1]= (uint64_t)&(mapping->startCode[0]);
}

Process::Process(int pid,std::string&& name,VirtualMapping* mapping,unsigned long ssize,int prio,const char** argv, const char** envp):
		pid(pid),name(std::move(name)),prio(prio),state(ProcessState::Ready),topStack(mapping->topStack),wakeUp(0),daddy(0),sonPid(0),
	retval(0),killed(false),mapping(mapping){
	mapping->initMainArgs(argv,envp);
	regSave[1]= (uint64_t)&(mapping->startStack[0]);
}

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

