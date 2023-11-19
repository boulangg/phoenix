/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "InterruptDispatcher.h"

#include <errno.h>

#include "asm/cpu.h"
#include "ExceptionHandlers.h"
#include "InterruptDescTable.h"
#include "KernelGlobals.h"
#include "SyscallHandler.h"

#include "dev/input/KeyboardDevice.h"

namespace kernel::core {

static void setupPIC()
{
    /* Initialize the master. */
    outb(0x20, 0x11); // Init command
    outb(0x21, 0x20); // Set offset (IRQ 0->7 use IDT[0x20->0x27])
    outb(0x21, 0x4);  // Set slave at IRQ2
    outb(0x21, 0x1);  // Set 8086 mode ?

    /* Initialize the slave. */
    outb(0xa0, 0x11); // Init command
    outb(0xa1, 0x28); // Set offset (IRQ 8->15 use IDT[0x28->0x2F]
    outb(0xa1, 0x2);  // Set master
    outb(0xa1, 0x1);  // Set 8086 mode ?

    /* Disable all IRQs */
    outb(0x21, 0xff);
    outb(0xa1, 0xff);

    /* Ack any bogus intrs by setting the End Of Interrupt bit. */
    outb(0x20, 0x20);
    outb(0xa0, 0x20);
}

static void enableIRQ(std::uint8_t irq)
{
    uint16_t port;
    uint8_t value;

    if (irq < 8) {
        port = 0x21;
    } else {
        port = 0xa1;
        irq -= 8;
    }
    value = inb(port) & ~(1 << irq);
    outb(port, value);
}

static void disableIRQ(std::uint8_t irq)
{
    uint16_t port;
    uint8_t value;

    if (irq < 8) {
        port = 0x21;
    } else {
        port = 0xa1;
        irq -= 8;
    }
    value = inb(port) | (1 << irq);
    outb(port, value);
}

InterruptDispatcher::InterruptDispatcher() {}

void InterruptDispatcher::init()
{
    setupPIC();
    ExceptionHandlers::setupHandlers();
    IDT::setupIDT();
    syscall::setupSyscall();

    dev::input::KeyboardDevice::initKeyboard();
}

void InterruptDispatcher::handleIRQ(std::uint8_t irq)
{
    // Is it PIC interrupt
    if (irq < 16) {
        // Does it come from slave PIC
        if (irq >= 8) {
            outb(0xA0, 0x20);
        }
        outb(0x20, 0x20);
    }

    if (!_handlers.contains(irq)) {
        return;
    }

    for (auto& handler : _handlers[irq]) {
        handler->operator()(irq);
    }
}

int InterruptDispatcher::registerHandler(std::uint8_t irq, InterruptHandler* handler)
{
    if (irq >= 16) {
        printk("Unsupported IRQ requested: %u\n", irq);
    }

    if (!handler->getFlags().SHARED && _handlers.contains(irq)) {
        return -EBUSY;
    }

    _handlers[irq].push_back(handler);
    enableIRQ(irq);
    if (irq > 7) {
        enableIRQ(2);
    }

    return 0;
}

void InterruptDispatcher::unregisterHandler(std::uint8_t irq, InterruptHandler* handler)
{
    if (!_handlers.contains(irq)) {
        return;
    }

    _handlers[irq].remove(handler);

    if (_handlers[irq].empty()) {
        _handlers.erase(irq);
        disableIRQ(irq);
    }
}

}
