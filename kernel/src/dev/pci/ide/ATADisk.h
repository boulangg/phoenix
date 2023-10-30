/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include "IDEChannel.h"
#include "IDEDisk.h"
#include "core/InterruptDispatcher.h"

namespace kernel::dev::pci::ide {

class ATADisk : public IDEDisk
{
public:
    ATADisk(IDEChannel* parent, std::uint32_t major, std::uint32_t minor, std::string name, std::uint8_t irqNum,
            bool isSlave, IDEIdentifyData identifyData);
    virtual ~ATADisk() {}

protected:
    ProcessRequestState processRequest(BlockIORequest& request) override;
    int IRQHandler(std::uint8_t, void*);

private:
    IDEChannel* _parent;
    std::uint8_t _irqNum;
    bool _isSlave;
    mem::Page* _prdt;
    IDEIdentifyData _identifyData;
    core::InterruptHandler* _handler;
};

}
