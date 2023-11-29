/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <cstddef>
#include <cstdint>
#include <list>
#include <vector>

#include "mem/Page.h"

namespace kernel::dev {

struct Device
{

};

struct BlockIORequestEntry
{
    mem::Page* page;
    std::size_t len;
    std::size_t offset;
};

struct BlockIORequest
{
    std::size_t start;
    std::vector<BlockIOREquestEntry> entries;
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
    virtual ~BlockDevice() {};

    BlockDevice(ProcessRequestState defaultState = None) : _state(defaultState) {}

    virtual void processRequests() = 0;

    ProcessRequestState getProcessState()
    {
        return _state;
    }

    void setProcessState(ProcessRequestState)
    {
        _state = state;
    }

    virtual void submitRequest(BlockIORequest entry) = 0;
    virtual bool hasPendingRequest() = 0;

private:
    ProcessRequestState _state;
};

class Partition : public BlockDevice
{
public:
    virtual ~Partition() {};

    Partition(BlockDevice* parent, std::size_t startSector) : BlockDevice(), _parent(parent), _startSector(startSector)
    {}

    void submitRequest(BlockIORequest request) override
    {
        request.start += _startSector;
        _parent->submitRequest(request);
    }

    void processRequests() override
    {
        return;
    }

    bool hasPendingRequest() override
    {
        return false;
    }

private:
    BlockDevice* _parent;
    std::size_t _startSector;
};

class Disk : public BlockDevice
{
public:
    virtual ~Disk() {};

    void submitRequest(BlockIORequest request) override
    {
        _requests.push_back(request);
        if (getProcessState() == ProcessRequestState::None) {
            setProcessState(ProcessRequestState::Pending);
        }
    }

    ProcessRequestState processRequests() override
    {
        switch (getProcessState()) {
        case ProcessRequestState::None:
        case ProcessRequestState::InProgress:
            break;
        case ProcessRequestState::Pending:
            auto state = processRequest();
            setProcessState(state);
            break;
        }
        return getProcessState();
    }

    bool hasPendingRequest() override
    {
        return !_requests.empty();
    }

protected:
    void dequeueFirstRequest()
    {
        _requests.pop_front();
        if (_requests.empty()) {
            setProcessState(ProcessRequestState::None);
        } else {
            setProcessState(ProcessRequestState::Pending);
        }
    }

    BlockIORequest& getFirstRequest()
    {
        return _requests.front();
    }

    virtual ProcessRequestState processRequest(BlockIORequest& request) = 0;

private:
    std::list<BlockIORequest> _requests;
    std::size_t _capacity; // sector count
    std::vector<Partition> _partitions;
};

class Ext2Disk : Disk
{
protected:
    ProcessRequestState processRequest() override
    {
        BlockIORequest& request = getFirstRequest();
        // Setup DMA for the request
        return ProcessRequestState::InProgress;
    }

    void handleIRQ()
    {
        BlockIORequest& request = getFirstRequest();
        // TODO verify if it's this device that trigger the IRQ

        // Check the progress of the request

        dequeueFirstRequest();
    }
};

}
