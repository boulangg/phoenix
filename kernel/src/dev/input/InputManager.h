/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <list>

#include "InputDevice.h"
#include "InputHandler.h"

namespace kernel::dev::input {

class InputManager
{
public:
    static void init();

    static void registerDevice(InputDevice* device)
    {
        _devices.push_back(device);
        for (auto handler : _handlers) {
            handler->connect(device);
        }
    }

    static void registerHandler(InputHandler* handler)
    {
        _handlers.push_back(handler);
        for (auto device : _devices) {
            handler->connect(device);
        }
    }

private:
    static std::list<InputHandler*> _handlers;
    static std::list<InputDevice*> _devices;
};

}
