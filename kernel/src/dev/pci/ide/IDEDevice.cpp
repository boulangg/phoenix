/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "IDEDevice.h"

#include "IDEChannel.h"

namespace kernel::dev::pci::ide {

IDEDevice::IDEDevice(PCIDevice* dev) : _dev(dev)
{
    initDrives(dev);
}

void IDEDevice::initDrives(PCIDevice* device)
{
    // Primary Channel
    IDEChannelRegisters primaryRegs = {};
    primaryRegs.base = device->getConfigSpace().BAR0 & 0xFFFFFFFC;
    if (primaryRegs.base == 0x00) {
        primaryRegs.base = 0x1F0;
    }
    primaryRegs.ctrl = device->getConfigSpace().BAR1 & 0xFFFFFFFC;
    if (primaryRegs.ctrl == 0x00) {
        primaryRegs.ctrl = 0x3F6;
    }
    primaryRegs.bmIDE = device->getConfigSpace().BAR4 & 0xFFF0;
    _channels[0] = new IDEChannel(this, true, primaryRegs, 14);
    _channels[0]->initDrives();

    // Secondary Channel
    IDEChannelRegisters secondaryRegs = {};
    secondaryRegs.base = device->getConfigSpace().BAR2 & 0xFFFFFFFC;
    if (secondaryRegs.base == 0x00) {
        secondaryRegs.base = 0x170;
    }
    secondaryRegs.ctrl = device->getConfigSpace().BAR3 & 0xFFFFFFFC;
    if (secondaryRegs.ctrl == 0x00) {
        secondaryRegs.ctrl = 0x376;
    }
    secondaryRegs.bmIDE = (device->getConfigSpace().BAR4 + 8) & 0xFFF0;
    _channels[1] = new IDEChannel(this, false, secondaryRegs, 15);
    _channels[1]->initDrives();
}

}
