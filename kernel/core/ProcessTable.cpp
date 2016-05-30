/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "ProcessTable.hpp"
#include <fs/Elf64.hpp>
#include <fs/KernelFS.hpp>
#include <fs/File.hpp>
#include <asm/cpu.h>
#include "Console.hpp"
#include "stdio.h"

//TODO move this somewhere else
int idle(int,char**,char**)
{
	for (;;)
	{
		sti();
		hlt();
		cli();
	}
	return 0;
}

uint64_t ProcessTable::nbProcess(0);
uint64_t ProcessTable::lastAssignedPid(-1);
Process* ProcessTable::running = nullptr;
std::vector<Process*> ProcessTable::processVector;

std::priority_queue<Process*,std::vector<Process*>,ProcessLess> ProcessTable::ready;

void ProcessTable::init(){
	initIdle();
	start("test1",512,5,42,nullptr,nullptr);
}

void ProcessTable::initIdle(){

	//TODO idle has some uninitialized members
	Process* proc = new Process(0,idle,"idle",512,0);
	if(proc==nullptr)
		return;
	ProcessTable::nbProcess = 1;
	ProcessTable::lastAssignedPid=0;
	ProcessTable::running=proc;
	ProcessTable::processVector.push_back(proc);
}

int ProcessTable::start(const char *name, unsigned long ssize, int prio, int argc,char** argv,char** envp){

	if (prio <= 0 || prio > MAXPRIO || name == NULL || ssize >= ((unsigned long)32000))
		return -1;

	File* f;
	VirtualMapping* mapping;
	f = KernelFS::getUserApp(name);
	if(f==nullptr)
		return -1;
	mapping = Elf64::getVirtualMapping(f);
	if(mapping==nullptr)
		return -1;
	PageTable* pgTable = mapping->getPageTable();
	if(pgTable==nullptr)
		return -1;
	PageTable::setActivePageTable(pgTable);

	int64_t pid = findPid();
	if (pid == -1)
		return -1;

	Process* proc= new Process(pid,name,*mapping,ssize,prio,argc,argv,envp);
	ready.push(proc);
	ProcessTable::nbProcess++;
	ProcessTable::lastAssignedPid=pid;
	//TODO switch
	return pid;
}

int64_t ProcessTable::findPid(){

	//TODO delete dying

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

void ProcessTable::schedule(){
	//TODO delete dying

	Process *currProc= running;
	running->setState(ProcessState::Ready);

	ready.push(currProc);

	unconditionalContextSwitch(currProc);
}

void ProcessTable::unconditionalContextSwitch(Process* currProc){
	Process *nextProc = ready.top();
	ready.pop();
	nextProc->setState(ProcessState::Running);
	running = nextProc;

	ctx_sw(currProc->getRegSave(), nextProc->getRegSave());
}

void ProcessTable::do_exit(int retval){
	Process *currProc= running;
	running->setState(ProcessState::Dying);
	char str[20];
	sprintf(str,"retval %d",retval);
	Console::write(str);
	//TODO add to dying queue
	unconditionalContextSwitch(currProc);
}
