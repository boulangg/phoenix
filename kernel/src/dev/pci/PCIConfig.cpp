/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #include "PCIConfig.h"

#include "asm/cpu.h"

namespace kernel::dev::pci {

static std::uint32_t getAddress(std::uint8_t bus, std::uint8_t dev, std::uint8_t func, std::uint8_t off)
{
    std::uint32_t address;
    std::uint32_t lbus = std::uint32_t(bus);
    std::uint32_t ldev = std::uint32_t(dev);
    std::uint32_t lfunc = std::uint32_t(func);
    std::uint32_t loff = std::uint32_t(off & 0xFC);

    address = (uint32_t(0x80000000) | (lbus << 16) | (ldev << 11) | (lfunc << 8) | (loff));

    return address;
}

static std::uint32_t readPCIConfigLine(uint8_t bus, uint8_t dev, uint8_t func, uint8_t off)
{
    uint32_t address = getAddress(bus, dev, func, off);

    /* write out the address */
    outl(0xCF8, address);
    /* read in the data */
    uint32_t tmp = inl(0xCFC);
    return tmp;
}

PCIConfigSpace::PCIConfigSpace(std::uint8_t bus, std::uint8_t dev, std::uint8_t func) : bus(bus), dev(dev), func(func)
{
    PCIConfigSpace0 = readPCIConfigLine(bus, dev, func, 0x00);
    PCIConfigSpace1 = readPCIConfigLine(bus, dev, func, 0x04);
    PCIConfigSpace2 = readPCIConfigLine(bus, dev, func, 0x08);
    PCIConfigSpace3 = readPCIConfigLine(bus, dev, func, 0x0C);
    PCIConfigSpace4 = readPCIConfigLine(bus, dev, func, 0x10);
    PCIConfigSpace5 = readPCIConfigLine(bus, dev, func, 0x14);
    PCIConfigSpace6 = readPCIConfigLine(bus, dev, func, 0x18);
    PCIConfigSpace7 = readPCIConfigLine(bus, dev, func, 0x1C);
    PCIConfigSpace8 = readPCIConfigLine(bus, dev, func, 0x20);
    PCIConfigSpace9 = readPCIConfigLine(bus, dev, func, 0x24);
    PCIConfigSpace10 = readPCIConfigLine(bus, dev, func, 0x28);
    PCIConfigSpace11 = readPCIConfigLine(bus, dev, func, 0x2C);
    PCIConfigSpace12 = readPCIConfigLine(bus, dev, func, 0x30);
    PCIConfigSpace13 = readPCIConfigLine(bus, dev, func, 0x34);
    PCIConfigSpace14 = readPCIConfigLine(bus, dev, func, 0x38);
    PCIConfigSpace15 = readPCIConfigLine(bus, dev, func, 0x3C);
}

std::uint16_t PCIConfigSpace::getVendorID(std::uint8_t bus, std::uint8_t dev, std::uint8_t func)
{
    return readPCIConfigLine(bus, dev, func, 0x00) & 0xFFFF;
}

std::uint8_t PCIConfigSpace::getHeaderType(std::uint8_t bus, std::uint8_t dev, std::uint8_t func)
{
    return (readPCIConfigLine(bus, dev, func, 0x0C) >> 16) & 0xFF;
}

bool PCIConfigSpace::checkClass(std::optional<std::uint8_t> classCode, std::optional<std::uint8_t> subClass,
                std::optional<std::uint8_t> progIf) const
{
    if (classCode.has_value() && this->classCode != classCode.value()) {
        return false;
    }

    if (subClass.has_value() && this->subclassCode != subClass.value()) {
        return false;
    }

    if (progIf.has_value() && this->progIF != progIf.value()) {
        return false;
    }

    return true;
}


}
