/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include "PCIDevice.h"

namespace kernel::dev::pci {

class PCIDriver
{
public:
    PCIDriver() {}
    virtual ~PCIDriver() {}

    virtual bool registerDevice(PCIDevice* device) = 0;
};

}
