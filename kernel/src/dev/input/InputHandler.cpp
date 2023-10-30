/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "InputHandler.h"

#include "InputDevice.h"

namespace kernel::dev::input {

int InputHandler::connect(InputDevice* dev)
{
    dev->registerHandler(this);
    return 0;
}

void InputHandler::disconnect(InputDevice* dev)
{
    dev->unregisterHandler(this);
}

}
