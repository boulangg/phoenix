/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "Partition.hpp"

#include "Disk.hpp"

Partition::Partition(Disk* storage, int partNo, std::string name) :
    BlockDevice(), storage(storage), partNo(partNo), name(name)
{}
Partition::~Partition() {}

std::uint64_t Partition::getStorageSize()
{
    return getSectorNumber() * getSectorSize();
}

std::uint64_t Partition::getSectorNumber()
{
    return storage->getPartitionInfo(partNo).nbSectors;
}

std::uint64_t Partition::getSectorSize()
{
    return storage->getSectorSize();
}

std::uint64_t Partition::getPartitionStart()
{
    return storage->getPartitionInfo(partNo).startSector;
}

Block* Partition::getBlock(std::uint64_t lba)
{
    return storage->getBlock(lba + getPartitionStart());
}

const std::string& Partition::getName() const
{
    return name;
}
