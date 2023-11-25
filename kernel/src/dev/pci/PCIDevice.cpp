/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #include "PCIDevice.h"

namespace kernel::dev::pci {

PCIDevice::PCIDevice(std::uint8_t bus, std::uint8_t slot, std::uint8_t function) : 
    _bus(bus), _slot(slot), _function(function), _configSpace(bus, slot, function) {}

}
