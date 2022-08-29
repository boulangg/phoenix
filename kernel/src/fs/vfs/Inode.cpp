#include "Inode.hpp"

#include "SuperBlock.hpp"
#include "DeviceFile.hpp"
#include <driver/DeviceManager.hpp>
#include <driver/CharacterDevice.hpp>

Inode::Inode(SuperBlock* sb, std::uint64_t ino, size_t size) :
	sb(sb), ino(ino), size(size), mapping(nullptr), dentries()
{

}

Inode::~Inode()
{}

Dentry* Inode::lookup(Dentry* parent, std::string name)
{
	(void)parent; (void)name;
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
