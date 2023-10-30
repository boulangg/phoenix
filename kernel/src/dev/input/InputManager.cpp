/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #include "InputManager.h"

namespace kernel::dev::input {

std::list<InputHandler*> InputManager::_handlers;
std::list<InputDevice*> InputManager::_devices;

}
