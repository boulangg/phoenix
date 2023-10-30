#pragma once

#include <list>

#include "InputDevice.hpp"
#include "InputHandler.hpp"

class InputManager
{
public:
	static void init();

	static void registerDevice(InputDevice* device)
	{
		_devices.push_back(device);
		for (auto handler : _handlers) {
			handler->connect(device, 0);
		}
	}

	static void registerHandler(InputHandler* handler)
	{
		_handlers.push_back(handler);
		for (auto device : _devices) {
			handler->connect(device, 0);
		}
	}

private:
	static std::list<InputHandler*> _handlers;
	static std::list<InputDevice*> _devices;
};
