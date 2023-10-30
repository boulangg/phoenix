/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #include "IDEDriver.h"

namespace kernel::dev::pci::ide {

bool IDEDriver::registerDevice(PCIDevice* device)
{
    if (!device->getConfigSpace().checkClass(0x01, 0x01, {})) {
        return false;
    }

    _devices.push_back(new IDEDevice(device));
    return true;
}

}
