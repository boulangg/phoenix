/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "InputDevice.h"

namespace kernel::dev::input {

InputDevice::InputDevice(std::string name) : _name(name), _handlers(), _values() {}

InputDevice::~InputDevice()
{
    for (auto handler : _handlers) {
        handler->disconnect(this);
    }
}

int InputDevice::handleIncomingEvent(InputEvent val)
{
    _values.push_back(val);
    if (val.type == EV_SYN) {
        for (auto handler : _handlers) {
            handler->handleEvents(_values);
        }
        _values.clear();
    }
    return 0;
}

}
