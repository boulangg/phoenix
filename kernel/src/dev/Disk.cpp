/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "Disk.h"

namespace kernel::dev {

Disk::Disk(std::uint32_t major, std::uint32_t minor, std::string name) : BlockDevice(major, minor, name) {}

Disk::~Disk(){};

void Disk::submitRequest(BlockIORequest request)
{
    _requests.push_back(request);
    if (getProcessState() == ProcessRequestState::None) {
        setProcessState(ProcessRequestState::Pending);
    }
}

ProcessRequestState Disk::processRequests()
{
    switch (getProcessState()) {
    case ProcessRequestState::None:
    case ProcessRequestState::InProgress:
        break;
    case ProcessRequestState::Pending:
        auto state = processRequest(_requests.front());
        setProcessState(state);
        break;
    }
    return getProcessState();
}

bool Disk::hasPendingRequest()
{
    return !_requests.empty();
}

void Disk::dequeueFirstRequest()
{
    _requests.pop_front();
    if (_requests.empty()) {
        setProcessState(ProcessRequestState::None);
    } else {
        setProcessState(ProcessRequestState::Pending);
    }
}

BlockIORequest& Disk::getFirstRequest()
{
    return _requests.front();
}

}
