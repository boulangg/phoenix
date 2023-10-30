/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "DeviceExplorer.h"

#include "KernelGlobals.h"
#include "pci/PCIManager.h"

namespace kernel::dev {

struct PartitionTableEntry
{
    std::uint8_t driveAttributes;
    std::uint32_t CHSAddressStart : 24;
    std::uint8_t partitionType;
    std::uint32_t CHSAddressEnd : 24;
    std::uint32_t startSector;
    std::uint32_t nbSectors;
} __attribute__((packed));

struct MBR
{
    char bootstrap[436];
    char diskID[10];
    PartitionTableEntry MBRPartitionTable[4];
    std::uint16_t signature;

    static const std::uint16_t SIGNATURE_MAGIC = 0xAA55;
} __attribute__((packed));

DeviceExplorer::DeviceExplorer() {}

void DeviceExplorer::init()
{
    pci::PCIManager::initPCI();
}

void DeviceExplorer::addDisk(Disk* disk)
{
    printk("Disk found - %lld bytes\n", disk->getBlockCount() * disk->getBlockSize());
    _blkDevs.push_back(disk);

    mem::Page* page = alloc_page();
    page->lock();
    disk->readPage(page, 0, 1);
    page->unlock();

    MBR* mbr = (MBR*)(page->getKernelAddr());
    if (mbr->signature == MBR::SIGNATURE_MAGIC) {
        printk("MBR found\n");
        for (int partNum = 0; partNum < 4; partNum++) {
            auto& entry = mbr->MBRPartitionTable[partNum];
            if (entry.partitionType == 0x00) {
                continue;
            }
            printk("Partition found - 0x%x\n", entry.partitionType);
            Partition* part = new Partition(disk, partNum, entry.startSector, entry.nbSectors);
            addDrive(part);

            printk("Partition %u (%s) size: %u sectors, start: %u\n", partNum, part->getName().c_str(), entry.nbSectors,
                   entry.startSector);
        }
    } else {
        printk("No MBR found\n");
    }

    free_page(page);
}

void DeviceExplorer::addDrive(BlockDevice* disk)
{
    _blkDevs.push_back(disk);
}

void DeviceExplorer::blockDeviceService()
{
    while (true) {
        for (auto& blkDev : _blkDevs) {
            if (blkDev->hasPendingRequest()) {
                blkDev->processRequests();
            }
        }
        schedule();
    }
}

}
