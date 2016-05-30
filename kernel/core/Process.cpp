/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "Process.hpp"

#include "ProcessTable.hpp"
#include "Console.hpp"

#include <utility>



Process::Process(int pid,code_type code,std::string&& name,unsigned long ssize,int prio):
	pid(pid),name(std::move(name)),prio(prio),state(ProcessState::Ready),topStack(new size_type[STACK_SIZE]),kernelStack(nullptr),wakeUp(0),daddy(0),sonPid(0),
	retval(0),killed(false){
	topStack[-3]=(uint64_t)code;
	regSave[1]= (uint64_t)&(topStack[-3]);
}

Process::Process(int pid,std::string&& name,const VirtualMapping& mapping,unsigned long ssize,int prio,int argc,char* argv[], char* envp[]):
		pid(pid),name(std::move(name)),prio(prio),state(ProcessState::Ready),topStack(mapping.topStack),kernelStack(nullptr),wakeUp(0),daddy(0),sonPid(0),
	retval(0),killed(false){
	topStack[-4]=(uint64_t)mapping.entryPoint;
	topStack[-3]=argc; //argc
	topStack[-2]=(uint64_t)argv; //argv
	topStack[-1]=(uint64_t)envp; //env
	regSave[1]= (uint64_t)&(topStack[-4]);
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

