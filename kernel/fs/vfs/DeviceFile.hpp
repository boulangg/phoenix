#pragma once

#include "File.hpp"
#include <driver/CharacterDevice.hpp>

class DeviceFile : public File {
public:
	DeviceFile(File* file, CharacterDevice* chardev) : File(),
		_file(file), _chardev(chardev) {

	}

	virtual ~DeviceFile() {

	}

	virtual Inode* getInode() override {
		return _file->getInode();
	}

	virtual int64_t lseek_internal(int64_t, uint32_t) override {
		return -EINVAL;
	}

	virtual ssize_t read_internal(char* buffer, size_t size, loff_t offset) override {
		return _chardev->read(buffer, size, offset);
	}

	virtual ssize_t write_internal(char* buffer, size_t size, loff_t offset) override {
		return _chardev->write(buffer, size, offset);
	}


private:
	File* _file;
	CharacterDevice* _chardev;
};
