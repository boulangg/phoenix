/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at
 * https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include <list>
#include <vector>

#include "BlockCache.h"
#include "BlockDevice.h"
#include "Partition.h"

#include "mem/Page.h"

namespace kernel::dev {

class Disk : public BlockDevice
{
public:
    Disk(std::uint32_t major, std::uint32_t minor, std::string name);
    virtual ~Disk();

    void submitRequest(BlockIORequest request) override;
    ProcessRequestState processRequests() override;
    bool hasPendingRequest() override;

protected:
    void dequeueFirstRequest();
    BlockIORequest& getFirstRequest();

    virtual ProcessRequestState processRequest(BlockIORequest& request) = 0;

private:
    std::list<BlockIORequest> _requests;
    std::size_t _capacity; // sector count
    std::vector<Partition> _partitions;
};

}
