
#pragma once

#include <fs/vfs/Inode.hpp>

class Ext2Inode : public Inode {
public:
	File* getFile();
};
