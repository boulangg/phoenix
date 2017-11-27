#include "DeviceManager.hpp"

#include "Disk.hpp"

std::list<BlockStorageDevice*> DeviceManager::_blockStorageDevices;

void DeviceManager::registerIDEDevice(BlockStorageDevice* dev) {
	_blockStorageDevices.push_back(dev);
	Disk disk(dev);
}

std::list<BlockStorageDevice*> DeviceManager::getAllStorageDevices() {
	return _blockStorageDevices;
}
