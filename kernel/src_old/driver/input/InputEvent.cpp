/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "InputEvent.hpp"

std::list<InputHandler*> InputManager::_handlers;
std::list<InputDevice*> InputManager::_devices;

InputDevice::~InputDevice()
{
    for (auto handler : handlers) {
        handler->disconnect(this);
    }
}

int InputDevice::handleIncomingEvent(InputValue val)
{
    values.push_back(val);
    if (val.type == EV_SYN) {
        for (auto handler : handlers) {
            handler->handleEvents(values);
        }
        values.clear();
    }
    return 0;
}
