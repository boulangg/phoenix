/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "BlockDevice.h"

#include <bit>

namespace kernel::dev {

BlockDevice::BlockDevice(std::uint32_t major, std::uint32_t minor, std::string name) :
    Device(DeviceType::BLK_DEVICE, major, minor, name), _blockSize(DEFAULT_BLOCK_SIZE),
    _blockSizeShift(std::countr_zero(_blockSize)), _state(ProcessRequestState::None), _blockCount(0)
{}

BlockDevice::~BlockDevice(){};

ProcessRequestState BlockDevice::getProcessState()
{
    return _state;
}

void BlockDevice::setProcessState(ProcessRequestState state)
{
    _state = state;
}

std::uint64_t BlockDevice::getBlockSize()
{
    return _blockSize;
}

std::uint8_t BlockDevice::getBlockShift()
{
    return _blockSizeShift;
}

std::uint64_t BlockDevice::getBlockCount()
{
    return _blockCount;
}

void BlockDevice::setBlockSize(std::uint64_t blockSize)
{
    _blockSize = blockSize;
    _blockSizeShift = std::countr_zero(blockSize);
}

void BlockDevice::setBlockCount(std::uint64_t blockCount)
{
    _blockCount = blockCount;
}

}
