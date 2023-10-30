/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "ATADisk.h"

#include "Kernel.h"
#include "KernelGlobals.h"

namespace kernel::dev::pci::ide {

ATADisk::ATADisk(IDEChannel* parent, std::uint32_t major, std::uint32_t minor, std::string name, std::uint8_t irqNum,
                 bool isSlave, IDEIdentifyData identifyData) :
    IDEDisk(major, minor, name),
    _parent(parent), _irqNum(irqNum), _isSlave(isSlave), _prdt(alloc_page()), _identifyData(identifyData)
{
    setBlockSize(DEFAULT_BLOCK_SIZE);

    // Read block count
    if (_identifyData.commandSetSupportedEnabled[1] & (1 << 10)) {
        // Device support 48 LBA
        setBlockCount(_identifyData.max48LBA);
    } else {
        // Device uses CHS or 28-bit LBA
        setBlockCount(_identifyData.numAdressableSector);
    }

    _handler = new core::InterruptHandlerClass<ATADisk, &ATADisk::IRQHandler>("ATA", {true, true}, nullptr, this);
}

struct IDEPrd
{
    std::uint32_t baseAddr;
    std::uint16_t byteCount;
    std::uint16_t reserved : 15;
    bool EOT               : 1;
} __attribute__((packed));

using IDEPrdt = IDEPrd*;

ProcessRequestState ATADisk::processRequest(BlockIORequest& request)
{
    // TODO: Fix DMA
#ifdef ENABLED_DMA
    IDEPrdt prdt = reinterpret_cast<IDEPrdt>(_prdt->getKernelAddr());
    std::size_t totalSize = 0;
    for (std::size_t i = 0; i < request.entries.size(); ++i) {
        auto& entry = request.entries[i];
        prdt[i].baseAddr = entry.page->getPhysicalAddr() + entry.offset;
        prdt[i].byteCount = entry.len;
        prdt[i].EOT = false;
        totalSize += entry.len;
    }
    prdt[request.entries.size() - 1].EOT = true;

    printk("DMA access: start %u - length %u - entries %u\n", request.start, totalSize, request.entries.size());

    Kernel::interrupt.registerHandler(_irqNum, _handler);

    _parent->executeDMARequest(_isSlave, _prdt, request.start, totalSize >> getBlockShift(), request.write);
    return ProcessRequestState::InProgress;
#else
    _parent->executePIORequest(request, getBlockShift(), _isSlave);
    for (auto& entry : request.entries) {
        entry.end();
    }
    dequeueFirstRequest();
    return ProcessRequestState::Pending;
#endif
}

int ATADisk::IRQHandler(std::uint8_t, void*)
{
    bool status = _parent->validateDMATransfer();
    if (status) {
        BlockIORequest& request = getFirstRequest();
        for (auto& entry : request.entries) {
            entry.end();
        }
        dequeueFirstRequest();
    }

    return 0;
}

}
