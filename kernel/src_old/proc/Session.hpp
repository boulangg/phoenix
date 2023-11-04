#ifndef __PROC_SESSION_HPP__
#define __PROC_SESSION_HPP__

#include <proc/ProcessGroup.hpp>
#include <cstdint>
#include <list>

class Session
{
public:
	Session(ProcessGroup* sessionLeaderGroup);
	~Session();

	void addProcessGroup(ProcessGroup*);
	void removeProcessGroup(ProcessGroup*);

	ProcessGroup* getSessionLeader();

	void writeKeyboardInput(char*);

private:
	int sid;
	ProcessGroup* sessionLeader;
	std::list<ProcessGroup*> procList;

};

#endif //__PROC_SESSION_HPP__
