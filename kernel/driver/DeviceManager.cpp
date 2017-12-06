#include "DeviceManager.hpp"

#include "Disk.hpp"

std::list<BlockStorageDevice*> DeviceManager::_blockStorageDevices;
std::list<BlockDevice*> DeviceManager::_blockDevices;

void DeviceManager::registerIDEDevice(BlockStorageDevice* dev) {
	_blockStorageDevices.push_back(dev);
	Disk disk(dev);
}

void DeviceManager::registerRamDisk(BlockStorageDevice* dev) {
	_blockStorageDevices.push_back(dev);
	Disk disk(dev);
}

void DeviceManager::registerBlockDevice(BlockDevice* dev) {
	_blockDevices.push_back(dev);
}

std::list<BlockStorageDevice*> DeviceManager::getAllStorageDevices() {
	return _blockStorageDevices;
}

std::list<BlockDevice*> DeviceManager::getAllBlockDevices() {
	return _blockDevices;
}
