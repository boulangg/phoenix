/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include <driver/PCI/IDE/IDEDriver.hpp>

std::vector<PCIDeviceID> IDEDriver::_deviceIds;

IDEDriver::IDEDriver()
{
    PCIDeviceID id1 = {.vendorID = PCI_ANY_ID,
                       .deviceID = PCI_ANY_ID,
                       .subVendorID = PCI_ANY_ID,
                       .subDeviceID = PCI_ANY_ID,
                       .classCode = 0x01,
                       .classCodeMask = PCI_ANY_ID};
    _deviceIds.push_back(id1);
}

const std::vector<PCIDeviceID> IDEDriver::getPCIDeviceID()
{
    return _deviceIds;
}

int IDEDriver::probe(PCIDevice* device)
{
    cout << "IDE device found\n";

    IDEDevice* IDEdevice = new IDEDevice(device, _IDEDevices.size());
    // IDEdevice->initDrives(device);
    _IDEDevices.push_back(IDEdevice);

    return 0;
}

void IDEDriver::remove(PCIDevice* device)
{
    (void)device;
}
