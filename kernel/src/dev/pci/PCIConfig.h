/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <cstdint>
#include <optional>

namespace kernel::dev::pci {

struct PCIConfigSpace
{
    union
    {
        std::uint32_t PCIConfigSpace0;
        struct
        {
            std::uint16_t deviceID;
            std::uint16_t vendorID;
        };
    };
    union
    {
        std::uint32_t PCIConfigSpace1;
        struct
        {
            std::uint16_t command;
            std::uint16_t status;
        };
    };
    union
    {
        std::uint32_t PCIConfigSpace2;
        struct
        {
            std::uint8_t revisionID;
            std::uint8_t progIF;
            std::uint8_t subclassCode;
            std::uint8_t classCode;
        };
        std::uint32_t deviceType;
    };
    union
    {
        std::uint32_t PCIConfigSpace3;
        struct
        {
            std::uint8_t cacheLineSize;
            std::uint8_t latencyTimer;
            std::uint8_t headerType;
            std::uint8_t BIST;
        };
    };
    union
    {
        std::uint32_t PCIConfigSpace4;
        std::uint32_t BAR0;
    };
    union
    {
        std::uint32_t PCIConfigSpace5;
        std::uint32_t BAR1;
    };
    union
    {
        std::uint32_t PCIConfigSpace6;
        std::uint32_t BAR2;
        struct
        {
            std::uint8_t primaryBusNumber;
            std::uint8_t secondaryBusNumber;
            std::uint8_t subordinateBusNumber;
            std::uint8_t secondaryLatencyTimer;
        };
    };
    union
    {
        std::uint32_t PCIConfigSpace7;
        std::uint32_t BAR3;
        struct
        {
            std::uint8_t IOBase;
            std::uint8_t IOLimit;
            std::uint16_t secondaryStatus;
        };
    };
    union
    {
        std::uint32_t PCIConfigSpace8;
        std::uint32_t BAR4;
        struct
        {
            std::uint16_t memoryBase;
            std::uint16_t memoryLimit;
        };
    };
    union
    {
        std::uint32_t PCIConfigSpace9;
        std::uint32_t BAR5;
    };
    union
    {
        std::uint32_t PCIConfigSpace10;
        std::uint32_t cardBusCISpointer;
    };
    union
    {
        std::uint32_t PCIConfigSpace11;
        struct
        {
            std::uint16_t subVendorID;
            std::uint16_t subDeviceID;
        };
    };
    union
    {
        std::uint32_t PCIConfigSpace12;
        std::uint32_t expansionROMBaseAdress;
    };
    union
    {
        std::uint32_t PCIConfigSpace13;
        union
        {
            std::uint8_t capabilitiesPointer;
            char reserved_0[3];
        };
    };
    union
    {
        std::uint32_t PCIConfigSpace14;
        char reserved_1[4];
    };
    union
    {
        std::uint32_t PCIConfigSpace15;
        struct
        {
            std::uint8_t interruptLine;
            std::uint8_t interruptPIN;
            std::uint8_t minGrant;
            std::uint8_t maxLatency;
        };
    };

    std::uint8_t bus;
    std::uint8_t dev;
    std::uint8_t func;

    PCIConfigSpace(std::uint8_t bus, std::uint8_t dev, std::uint8_t func);

    static std::uint16_t getVendorID(std::uint8_t bus, std::uint8_t dev, std::uint8_t func);
    static std::uint8_t getHeaderType(std::uint8_t bus, std::uint8_t dev, std::uint8_t func);

    bool checkClass(std::optional<std::uint8_t> classCode, std::optional<std::uint8_t> subClass,
                    std::optional<std::uint8_t> progIf) const;
};


}
