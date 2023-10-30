/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "DeviceManager.hpp"

#include "Disk.hpp"

std::list<BlockStorageDevice*> DeviceManager::_blockStorageDevices;
std::list<BlockDevice*> DeviceManager::_blockDevices;
std::list<CharacterDevice*> DeviceManager::_characterDevices;

void DeviceManager::registerIDEDevice(BlockStorageDevice* dev)
{
    _blockStorageDevices.push_back(dev);
    Disk* disk = new Disk(dev);
    (void)disk;
}

void DeviceManager::registerRamDisk(BlockStorageDevice* dev)
{
    _blockStorageDevices.push_back(dev);
    Disk* disk = new Disk(dev);
    (void)disk;
}

void DeviceManager::registerBlockDevice(BlockDevice* dev)
{
    _blockDevices.push_back(dev);
}

void DeviceManager::registerCharacterDevice(CharacterDevice* dev)
{
    _characterDevices.push_back(dev);
}

std::list<BlockStorageDevice*> DeviceManager::getAllStorageDevices()
{
    return _blockStorageDevices;
}

std::list<BlockDevice*> DeviceManager::getAllBlockDevices()
{
    return _blockDevices;
}
