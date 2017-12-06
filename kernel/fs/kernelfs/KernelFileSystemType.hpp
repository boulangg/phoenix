#pragma once

#include <fs/vfs/VirtualFileSystem.hpp>

#include "KernelSuperBlock.hpp"

class KernelFileSystemType : public FileSystemType {

public:
	KernelFileSystemType() : FileSystemType("kernel") {}

	SuperBlock* readSuperBlock(Dentry* source, const void* data) override {
		(void)data;
		SuperBlock* sb = new KernelSuperBlock(this, source);
		supers.push_back(sb);
		return sb;
	}

};
