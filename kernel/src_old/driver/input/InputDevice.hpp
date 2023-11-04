#pragma once

#include <cstdint>
#include <list>
#include <string>
#include <vector>

#include "InputEventCode.hpp"

class InputHandler;

class InputDevice
{
public:
	InputDevice(std::string name) : name(name), handlers(), values()
	{}
	virtual ~InputDevice();

	virtual int handleEvent(unsigned int, unsigned int, int)
	{
		return 0;
	}

	int handleIncomingEvent(uint16_t type, uint16_t code, int32_t value)
	{
		InputValue val = { type, code, value };
		return handleIncomingEvent(val);
	}

	int reportKey(uint16_t code, int32_t value)
	{
		return handleIncomingEvent(EV_KEY, code, value);
	}

	int reportRel(uint16_t code, int32_t value)
	{
		return handleIncomingEvent(EV_REL, code, value);
	}

	int reportAbs(uint16_t code, int32_t value)
	{
		return handleIncomingEvent(EV_ABS, code, value);
	}

	int reportFFStatus(uint16_t code, int32_t value)
	{
		return handleIncomingEvent(EV_FF_STATUS, code, value);
	}

	int reportSwitch(uint16_t code, int32_t value)
	{
		return handleIncomingEvent(EV_SW, code, value);
	}

	void sync()
	{
		handleIncomingEvent(EV_SYN, SYN_REPORT, 0);
	}

	void MTsync()
	{
		handleIncomingEvent(EV_SYN, SYN_MT_REPORT, 0);
	}

	void registerHandler(InputHandler* handler)
	{
		handlers.push_back(handler);
	}

	std::string name;

	std::list<InputHandler*> handlers;
	std::vector<InputValue> values;

private:
	int handleIncomingEvent(InputValue val);
};
