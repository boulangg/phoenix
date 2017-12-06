#pragma once

#include <include/constant.h>

#include "RamDisk.hpp"
#include <driver/DeviceManager.hpp>

class RamDiskManager {

public:
	static void initRamDisk() {
		RamDisk* initrd = new RamDisk((uint64_t)INITRD_START, (uint64_t)INITRD_END, 512);
		initrd->setName("initrd");
		DeviceManager::registerRamDisk(initrd);
	}
};
