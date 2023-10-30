/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

#include "mem/Page.h"

namespace kernel::dev {

enum DeviceType
{
    BLK_DEVICE,
    CHR_DEVICE
};

class Device
{
public:
    Device(DeviceType type, std::uint32_t major, std::uint32_t minor, std::string name) :
        _type(type), _major(major), _minor(minor), _name(name)
    {}

    DeviceType getType() const
    {
        return _type;
    }

    std::uint32_t getMajor() const
    {
        return _major;
    }

    std::uint32_t getMinor() const
    {
        return _minor;
    }

    const std::string& getName() const
    {
        return _name;
    }

private:
    DeviceType _type;
    std::uint32_t _major;
    std::uint32_t _minor;
    std::string _name;
};

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

class BlockDevice;

struct BlockDescriptor
{
    BlockDevice* blkDev;
    std::uint64_t blkNum;
    std::uint64_t blkSize;
    mem::Page* page;
    std::uint64_t offset;

    std::uint64_t getKernelAddr()
    {
        return page->getKernelAddr() + offset;
    }
};

class BlockDevice : public Device
{
public:
    BlockDevice(std::uint32_t major, std::uint32_t minor, std::string name);
    virtual ~BlockDevice();

    virtual ProcessRequestState processRequests() = 0;

    ProcessRequestState getProcessState();
    void setProcessState(ProcessRequestState state);
    std::uint64_t getBlockSize();
    std::uint8_t getBlockShift();
    std::uint64_t getBlockCount();

    virtual void submitRequest(BlockIORequest entry) = 0;
    virtual bool hasPendingRequest() = 0;

    void readPage(mem::Page* p, std::uint64_t blkNum, std::uint64_t blkCount)
    {
        kernel::dev::BlockIORequest request;
        request.start = blkNum;
        request.write = false;
        request.entries.push_back(BlockIORequestEntry{
            .page = p, .len = blkCount * getBlockSize(), .offset = 0, .end = [p]() { p->unlock(); }});
        submitRequest(request);
        p->lock();
    }

    void readBlocks(mem::Page* p, std::vector<std::uint64_t> blkNums)
    {
        p->setCounter(blkNums.size());
        for (std::size_t i = 0; i < blkNums.size(); ++i) {
            kernel::dev::BlockIORequest request;
            request.start = blkNums[i];
            request.write = false;
            request.entries.push_back(BlockIORequestEntry{.page = p,
                                                          .len = getBlockSize(),
                                                          .offset = i * getBlockSize(),
                                                          .end = [p]()
                                                          {
                                                              auto counter = p->decrementCounter();
                                                              if (counter == 0) {
                                                                  p->unlock();
                                                              }
                                                          }});
            submitRequest(request);
            p->lock();
        }
    }

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
