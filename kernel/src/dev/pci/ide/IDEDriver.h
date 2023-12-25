/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <list>

#include "IDEDevice.h"

#include "dev/pci/PCIDriver.h"

namespace kernel::dev::pci::ide {

class IDEDriver : public PCIDriver
{
public:
    bool registerDevice(PCIDevice* device) override;

    std::list<IDEDevice*> _devices;
};

}
