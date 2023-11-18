/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include <list>
#include <string>
#include <vector>

#include "InputEvent.h"
#include "InputEventCode.h"
#include "InputHandler.h"
#include "Kernel.h"

namespace kernel::dev::input {

class InputDevice
{
public:
    InputDevice(std::string name);
    virtual ~InputDevice();

    virtual int handleEvent(unsigned int, unsigned int, int)
    {
        return 0;
    }

    int handleIncomingEvent(uint16_t type, uint16_t code, int32_t value)
    {
        InputEvent val = {Kernel::getDateTime(), type, code, value};
        return handleIncomingEvent(val);
    }

    int reportMsc(uint16_t code, int32_t value)
    {
        return handleIncomingEvent(EV_MSC, code, value);
    }

    int reportKey(uint16_t code, int32_t value)
    {
        return handleIncomingEvent(EV_KEY, code, value);
    }

    int reportRel(uint16_t code, int32_t value)
    {
        return handleIncomingEvent(EV_REL, code, value);
    }

    int reportAbs(uint16_t code, int32_t value)
    {
        return handleIncomingEvent(EV_ABS, code, value);
    }

    int reportFFStatus(uint16_t code, int32_t value)
    {
        return handleIncomingEvent(EV_FF_STATUS, code, value);
    }

    int reportSwitch(uint16_t code, int32_t value)
    {
        return handleIncomingEvent(EV_SW, code, value);
    }

    void sync()
    {
        handleIncomingEvent(EV_SYN, SYN_REPORT, 0);
    }

    void MTsync()
    {
        handleIncomingEvent(EV_SYN, SYN_MT_REPORT, 0);
    }

    void registerHandler(InputHandler* handler)
    {
        _handlers.push_back(handler);
    }

    void unregisterHandler(InputHandler* handler)
    {
        _handlers.remove(handler);
    }

private:
    int handleIncomingEvent(InputEvent val);

    std::string _name;
    std::list<InputHandler*> _handlers;
    std::vector<InputEvent> _values;
};

}
