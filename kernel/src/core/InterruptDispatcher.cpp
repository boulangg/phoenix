/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "InterruptDispatcher.h"

#include "CpuInstructions.h"
#include "InterruptDescTable.h"
#include "SyscallHandler.h"

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

InterruptDispatcher::InterruptDispatcher() {}

void InterruptDispatcher::init()
{
    IDT::setupIDT();
    setupPIC();
    syscall::setupSyscall();
}

void InterruptDispatcher::handleIRQ(std::uint8_t irq)
{
    (void)irq;
    while (1) {
    }
}

}
