/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "IDEDrive.hpp"

#include "IDEChannel.hpp"
#include "IDEDriver.hpp"

IDEDrive::IDEDrive(IDEChannel* channel, bool slave, IDEDiskType type, int channelNo) :
    channel(channel), slave(slave), type(type), identifyData(), nbSectors(0), sectorSize(DEFAULT_SECTOR_SIZE)
{
    setName(IDEDriver::getNameForDrive(channelNo, slave));
}

IDEDrive::~IDEDrive() {}

void IDEDrive::processBlockIO(BlockIO bio)
{
    channel->processBlockIO(bio, slave);
}

std::uint64_t IDEDrive::getStorageSize()
{
    return nbSectors * sectorSize;
}

std::uint32_t IDEDrive::getSectorSize()
{
    return sectorSize;
}
