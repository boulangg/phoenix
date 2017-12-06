/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "Kernel.hpp"

#include <proc/ProcessScheduler.hpp>
#include <asm/cpu.h>

#include <driver/PCI/IDE/IDEDriver.hpp>
#include <driver/PCI/PCIManager.hpp>

#include <driver/ramdisk/RamDiskManager.hpp>

#include <fs/vfs/VirtualFileSystem.hpp>

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
	VirtualFileSystem::initVFS();

	IDEDriver* ide = new IDEDriver();
	PCIManager::registerDriver(ide);

	PCIManager::initPCI();
	RamDiskManager::initRamDisk();

	//VirtualFileSystem::mount("initrd", "/", "kernel", 0, nullptr);

	ProcessScheduler::init();

	idle();
	while(1)
		hlt();
	return;

}

