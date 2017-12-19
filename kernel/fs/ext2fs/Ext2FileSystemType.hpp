#pragma once

#include <fs/vfs/VirtualFileSystem.hpp>

#include "Ext2SuperBlock.hpp"

class Ext2FileSystemType : public FileSystemType {
public:
	Ext2FileSystemType() : FileSystemType("ext2") {

	}

	SuperBlock* readSuperBlock(const std::string& source, const void* data) override {
		Ext2SuperBlock* sb = new Ext2SuperBlock(this, source);
		if (sb->_valid) {
			SuperBlock* simplesb = sb;
			supers.push_back(simplesb);
			return sb;
		} else {
			delete sb;
			return nullptr;
		}
	}

};
