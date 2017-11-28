/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "Kernel.hpp"

#include <proc/ProcessScheduler.hpp>
#include <asm/cpu.h>
#include <driver/PCI/PCIManager.hpp>

#include <driver/PCI/IDE/IDEDriver.hpp>
#include <driver/PCI/PCIManager.hpp>

struct MBR {
	char bootstrap[436];
	char diskID[10];
	char MBRPartitionTable[64];
	char signature[2];
};

static_assert(sizeof(MBR) == 512, "Size is not correct");

struct Partition {
	union {
		struct {
			uint8_t partEntry[16];
		};
		struct {
			std::uint8_t bootIndicator;
			std::uint8_t startHead;
			std::uint8_t startSector;
			std::uint8_t startCylinder;
			std::uint8_t SystemID;
			std::uint8_t endHead;
			std::uint8_t endSector;
			std::uint8_t endCylinder;
			std::uint32_t partitionStartLBA;
			std::uint32_t partitionSize;
		};
	};
};

static_assert(sizeof(Partition) == 16, "Size is not correct");

void Kernel::Start() {

	IDEDriver* ide = new IDEDriver();
	PCIManager::registerDriver(ide);

	ProcessScheduler::init();
	PCIManager::initPCI();

	/*BlockIO bio;
	BlockIOSegment bioSeg1;
	bioSeg1.page = PhysicalAllocator::allocPage();
	bioSeg1.offset = 0;
	bioSeg1.len = 512;
	bio.segments.push_back(bioSeg1);
	bio.nb_sectors = 1;
	bio.start_sector = 0;
	bio.write = 0;
	for (auto drive: PCIManager::getAllStorageDevices()) {
		drive->loadBlockIO(bio);
		MBR* mbr = (MBR*)(bioSeg1.page->kernelMappAddr);
		for (int partNum = 0; partNum < 4; partNum++) {
			Partition part;
			for (int i = 0; i < 16; i++) {
				part.partEntry[i] = mbr->MBRPartitionTable[partNum*16+i];
			}
			char tmp[1024];
			sprintf(tmp, "Partition %i size: %i sectors, start: %i\n", partNum,
					part.partitionSize, part.partitionStartLBA);
			Console::write(tmp);
		}
		BlockIO bio;
		drive->loadBlockIO(bio);
	}*/

	idle();
	while(1)
		hlt();
	return;

}

