#pragma once

#include <list>

#include "BlockStorageDevice.hpp"
#include "BlockDevice.hpp"

class DeviceManager {
public:


	static void registerIDEDevice(BlockStorageDevice* dev);
	static void registerRamDisk(BlockStorageDevice* dev);
	static void registerBlockDevice(BlockDevice* dev);

	static BlockDevice* getBlockDevice(std::string name) {
		for (auto blk : _blockDevices) {
			if (blk->getName().compare(name) == 0) {
				return blk;
			}
		}
		return nullptr;
	}

	static std::list<BlockStorageDevice*> getAllStorageDevices();
	static std::list<BlockDevice*> getAllBlockDevices();

private:
	static std::list<BlockStorageDevice*> _blockStorageDevices;
	static std::list<BlockDevice*> _blockDevices;
};
