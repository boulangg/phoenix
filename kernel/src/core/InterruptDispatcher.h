/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include <cstdint>
#include <list>
#include <unordered_map>
#include <unordered_set>

namespace kernel::core {

struct InterruptFlags
{
    bool FAST   : 1;
    bool SHARED : 1;
};

class InterruptHandler
{
public:
    InterruptHandler(std::string name, InterruptFlags flags, void* data) : _name(name), _flags(flags), _data(data) {}

    virtual ~InterruptHandler() {}

    virtual int operator()(std::uint8_t irq) = 0;

    const std::string& getName()
    {
        return _name;
    }

    const InterruptFlags& getFlags()
    {
        return _flags;
    }

protected:
    std::string _name;
    InterruptFlags _flags;
    void* _data;
};

template <class Obj, int (Obj::*handler_function)(std::uint8_t, void*)>
class InterruptHandlerClass : public InterruptHandler
{
public:
    // typedef typename Obj::handler_function handler_type;
    InterruptHandlerClass(std::string name, InterruptFlags flags, void* data, Obj* obj) :
        InterruptHandler(name, flags, data), _obj(obj)
    {}

    virtual ~InterruptHandlerClass() {}

    int operator()(std::uint8_t irq) override
    {
        return (_obj->*handler_function)(irq, _data);
    }

    Obj* _obj;
};

template <int (*handler_function)(std::uint8_t, void*)>
class InterruptHandlerFunction : public InterruptHandler
{
public:
    // typedef handler_function handler_type;
    InterruptHandlerFunction(std::string name, InterruptFlags flags, void* data) : InterruptHandler(name, flags, data)
    {}

    virtual ~InterruptHandlerFunction() {}

    int operator()(std::uint8_t irq) override
    {
        return handler_function(irq, _data);
    }
};

class InterruptDispatcher
{
public:
    InterruptDispatcher();

    void init(); // Init idt and PIC

    void handleIRQ(std::uint8_t irq);

    using interruptHandler = void (*)(std::uint8_t, void*);
    int registerHandler(std::uint8_t irq, InterruptHandler* handler);
    void unregisterHandler(std::uint8_t irq, InterruptHandler* handler);

private:
    std::unordered_map<std::uint8_t, std::list<InterruptHandler*>> _handlers;
};

}
