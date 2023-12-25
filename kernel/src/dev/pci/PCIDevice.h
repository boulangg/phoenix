/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once 

#include <cstdint>

#include "PCIConfig.h"

namespace kernel::dev::pci {

class PCIDevice
{
public:
    PCIDevice(std::uint8_t bus, std::uint8_t slot, std::uint8_t function);

    const PCIConfigSpace& getConfigSpace() const
    {
        return _configSpace;
    }

private:
    std::uint8_t _bus;
    std::uint8_t _slot;
    std::uint8_t _function;
    PCIConfigSpace _configSpace;
};

}
