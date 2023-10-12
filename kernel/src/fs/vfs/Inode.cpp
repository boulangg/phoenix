/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "Inode.hpp"

#include "DeviceFile.hpp"
#include "SuperBlock.hpp"
#include <driver/CharacterDevice.hpp>
#include <driver/DeviceManager.hpp>

Inode::Inode(SuperBlock* sb, std::uint64_t ino, size_t size) :
    sb(sb), ino(ino), size(size), mapping(nullptr), dentries()
{}

Inode::~Inode() {}

Dentry* Inode::lookup(Dentry* parent, std::string name)
{
    (void)parent;
    (void)name;
    return nullptr;
}

File* Inode::open()
{
    mode_t mode = getMode();
    if (S_ISCHR(mode)) {
        dev_t devID = getDeviceID();
        CharacterDevice* cdev = DeviceManager::getCharacterDevice(devID);
        /*if (cdev == nullptr) {
            return nullptr;
        }*/
        File* f = open_internal();
        f = new DeviceFile(f, cdev);
        return f;
    } else {
        return open_internal();
    }
}
