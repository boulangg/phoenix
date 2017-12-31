#pragma once

#include <list>
#include <vector>

#include "InputEventCode.hpp"

class InputDevice;

class InputHandler {
public:
	InputHandler() : _devices() {

	}
	virtual ~InputHandler() {

	}

	virtual void handleEvents(const std::vector<InputValue>& vals) = 0;
	//bool (*filter)(struct input_handle *handle, unsigned int type, unsigned int code, int value);
	//bool (*match)(struct input_handler *handler, struct input_dev *dev);
	virtual int connect(InputDevice *dev, const struct input_device_id *id);

	virtual void disconnect(InputDevice *handle);

	//virtual void (*start)(struct input_handle *handle);

	std::list<InputDevice*> _devices;
};
