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

#include <core/Clock.hpp>
#include <driver/TTYDevice.hpp>
#include <driver/input/KeyboardDevice.hpp>

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
	Clock::init();
	KeyboardDevice::initKeyboard();

	VirtualFileSystem::initVFS();

	TTYDevice* ttydev = new TTYDevice(0x0500);
	DeviceManager::registerCharacterDevice(ttydev);

	ProcessScheduler::init();
}

