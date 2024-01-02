/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <cstdint>
#include <vector>
#include <functional>

#include "mem/Page.h"

namespace kernel::dev {

struct Device
{};

struct BlockIORequestEntry
{
    mem::Page* page;
    std::size_t len;
    std::size_t offset;
    std::function<void()> end;
};

struct BlockIORequest
{
    std::size_t start;
    std::vector<BlockIORequestEntry> entries;
    bool write;
};

enum ProcessRequestState
{
    None,
    Pending,
    InProgress
};

class BlockDevice : public Device
{
public:
    BlockDevice();
    virtual ~BlockDevice();

    virtual ProcessRequestState processRequests() = 0;

    ProcessRequestState getProcessState();
    void setProcessState(ProcessRequestState state);
    std::uint64_t getBlockSize();
    std::uint8_t getBlockShift();

    virtual void submitRequest(BlockIORequest entry) = 0;
    virtual bool hasPendingRequest() = 0;

protected:
    static constexpr std::uint64_t DEFAULT_BLOCK_SIZE = 1 << 9;

    void setBlockSize(std::uint64_t blockSize);
    void setBlockCount(std::uint64_t blockCount);

private:
    std::uint64_t _blockSize;
    std::uint8_t _blockSizeShift;
    ProcessRequestState _state;
    std::uint64_t _blockCount;
};

}
