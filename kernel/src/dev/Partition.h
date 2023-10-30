/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include <cstdint>

#include "BlockDevice.h"

namespace kernel::dev {

class Partition : public BlockDevice
{
public:
    Partition() : BlockDevice(0, 0, "") {}

    virtual ~Partition(){};

    Partition(BlockDevice* parent, std::uint32_t partNum, std::size_t startSector, std::size_t nbSector) :
        BlockDevice(parent->getMajor(), parent->getMinor() + partNum, parent->getName() + std::to_string(partNum)),
        _parent(parent), _startSector(startSector), _nbSector(nbSector)
    {
        setBlockSize(parent->getBlockSize());
    }

    void submitRequest(BlockIORequest request) override
    {
        request.start += _startSector;
        _parent->submitRequest(request);
    }

    ProcessRequestState processRequests() override
    {
        return ProcessRequestState::None;
    }

    bool hasPendingRequest() override
    {
        return false;
    }

private:
    BlockDevice* _parent;
    std::size_t _startSector;
    std::size_t _nbSector;
};

}
