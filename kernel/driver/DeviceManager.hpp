#pragma once

#include <list>

#include "BlockStorageDevice.hpp"

class DeviceManager {
public:


	static void registerIDEDevice(BlockStorageDevice* dev);

	static std::list<BlockStorageDevice*> getAllStorageDevices();
	static std::list<BlockStorageDevice*> getAllBlockDevices();

private:
	static std::list<BlockStorageDevice*> _blockStorageDevices;
};
