/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include "dev/pci/PCIDevice.h"

namespace kernel::dev::pci::ide {

class IDEChannel;

class IDEDevice
{
public:
    IDEDevice(PCIDevice* dev);

    void initDrives(PCIDevice* device);

private:
    static constexpr std::uint8_t IDE_CHANNEL_PER_DEVICE = 2;

    PCIDevice* _dev;
    IDEChannel* _channels[IDE_CHANNEL_PER_DEVICE];
};

}
