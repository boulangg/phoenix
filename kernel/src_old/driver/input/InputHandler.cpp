/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "InputHandler.hpp"
#include "InputDevice.hpp"

int InputHandler::connect(InputDevice* dev, const struct input_device_id* id)
{
    (void)id;
    dev->registerHandler(this);
    return 0;
}

void InputHandler::disconnect(InputDevice* dev)
{
    (void)dev;
    // TODO diconnect InputHandler
}
