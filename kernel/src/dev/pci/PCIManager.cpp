/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
#include "PCIManager.h"

#include <list>

#include "KernelGlobals.h"

#include "PCIConfig.h"
#include "PCIDevice.h"
#include "PCIDriver.h"

#include "ide/IDEDriver.h"

namespace kernel::dev::pci {

static std::list<PCIDriver*> _drivers;

static void printDeviceInfo(const PCIConfigSpace& configSpace)
{
    uint32_t bus = configSpace.bus;
    uint32_t dev = configSpace.dev;
    uint32_t func = configSpace.func;
    uint32_t deviceType = configSpace.deviceType;
    const char* deviceName;
    switch (deviceType >> 8) {
    case 0x010180:
        deviceName = "Storage    | IDE Controller - ISA Compatibility mode-only, supports bus mastering";
        break;
    case 0x010601:
        deviceName = "Storage    | Serial ATA Controller - AHCI 1.0";
        break;
    case 0x020000:
        deviceName = "Network    | Ethernet Controller";
        break;
    case 0x030000:
        deviceName = "Display    | VGA-Compatible Controller - VGA Controller";
        break;
    case 0x040100:
        deviceName = "Multimedia | Multimedia Audio Controller";
        break;
    case 0x040300:
        deviceName = "Multimedia | Audio Device";
        break;
    case 0x060000:
        deviceName = "Bridge     | Host Bridge";
        break;
    case 0x060100:
        deviceName = "Bridge     | ISA Bridge";
        break;
    case 0x068000:
        deviceName = "Bridge     | Other";
        break;
    case 0x0C0500:
        deviceName = "Serial Bus | SMBus Controller";
        break;
    default:
        deviceName = "Unknwon";
        break;
    }

    printk("pci-id: %0.2x:%0.2x:%0.2x, device: 0x%0.8x - %s\n", bus, dev, func, deviceType, deviceName);
}

static void checkBus(std::uint8_t bus);

static PCIConfigSpace checkFunc(std::uint8_t bus, std::uint8_t dev, std::uint8_t func)
{
    PCIDevice* device = new PCIDevice(bus, dev, func);
    const PCIConfigSpace& configSpace = device->getConfigSpace();
    printDeviceInfo(configSpace);

    // PCI-to-PCI bridge
    if ((configSpace.classCode == 0x6) && (configSpace.subclassCode == 0x4)) {
        checkBus(configSpace.secondaryBusNumber);
        return configSpace;
    }

    for (auto& driver : _drivers) {
        if (driver->registerDevice(device)) {
            continue;
        }
    }

    return configSpace;
}

static void checkDevice(std::uint8_t bus, std::uint8_t dev)
{
    std::uint32_t vendorID = PCIConfigSpace::getVendorID(bus, dev, 0);
    if (vendorID == 0xFFFF) {
        return; // No device
    }

    PCIConfigSpace configSpace = checkFunc(bus, dev, 0);

    // Check if multifunc device
    if ((configSpace.headerType & 0x80) != 0) {
        for (std::uint8_t func = 1; func < 8; func++) {
            std::uint32_t vendorID_fn = PCIConfigSpace::getVendorID(bus, dev, func);
            if ((vendorID_fn & 0xFFFF) == 0xFFFF) {
                continue; // No device
            }
            checkFunc(bus, dev, func);
        }
    }
}

static void checkBus(std::uint8_t bus)
{
    for (std::uint8_t device = 0; device < 32; device++) {
        checkDevice(bus, device);
    }
}

static void checkAllBuses()
{
    std::uint8_t headerType = PCIConfigSpace::getHeaderType(0, 0, 0);
    if ((headerType & 0x80) == 0) {
        // Single PCI host controller
        checkBus(0);
    } else {
        // Multiple PCI host controllers
        for (std::uint8_t function = 0; function < 8; function++) {
            if (PCIConfigSpace::getVendorID(0, 0, function) != 0xFFFF)
                break;
            std::uint8_t bus = function;
            checkBus(bus);
        }
    }
}

void PCIManager::initPCI()
{
    _drivers.push_back(new ide::IDEDriver());

    checkAllBuses();
}

}
