#include "InputHandler.hpp"
#include "InputDevice.hpp"

int InputHandler::connect(InputDevice *dev, const struct input_device_id *id) {
	(void)id;
	dev->registerHandler(this);
	return 0;
}

void InputHandler::disconnect(InputDevice *dev) {
	(void)dev;
	// TODO diconnect InputHandler
}
