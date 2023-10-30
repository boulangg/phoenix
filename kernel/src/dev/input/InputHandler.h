/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <list>
#include <vector>

#include "InputEvent.h"
#include "InputEventCode.h"

namespace kernel::dev::input {

class InputDevice;

class InputHandler
{
public:
    InputHandler() : _devices() {}
    virtual ~InputHandler() {}

    virtual void handleEvents(const std::vector<InputEvent>& vals) = 0;
    int connect(InputDevice* dev);
    void disconnect(InputDevice* handle);

    std::list<InputDevice*> _devices;
};

}
