#pragma once

#include <list>

#include "BlockStorageDevice.hpp"
#include "BlockDevice.hpp"
#include "CharacterDevice.hpp"

class DeviceManager
{
public:


	static void registerIDEDevice(BlockStorageDevice* dev);
	static void registerRamDisk(BlockStorageDevice* dev);
	static void registerBlockDevice(BlockDevice* dev);
	static void registerCharacterDevice(CharacterDevice* dev);

	static BlockDevice* getBlockDevice(std::string name)
	{
		for (auto blk : _blockDevices) {
			if (blk->getName().compare(name) == 0) {
				return blk;
			}
		}
		return nullptr;
	}

	static std::list<BlockStorageDevice*> getAllStorageDevices();
	static std::list<BlockDevice*> getAllBlockDevices();

	static CharacterDevice* getCharacterDevice(dev_t devID)
	{
		for (auto dev : _characterDevices) {
			if (dev->getDeviceID() == devID) {
				return dev;
			}
		}
		return nullptr;
	}

private:
	static std::list<BlockStorageDevice*> _blockStorageDevices;
	static std::list<BlockDevice*> _blockDevices;
	static std::list<CharacterDevice*> _characterDevices;
};
