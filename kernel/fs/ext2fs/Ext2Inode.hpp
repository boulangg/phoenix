
#pragma once

#include <fs/ext2fs/Ext2FSInfo.hpp>

#include "Ext2File.hpp"

class Ext2Inode : public BaseInode<Ext2FSInfo> {
public:
	Ext2Inode(Ext2SuperBlock* sb, std::uint64_t ino, ext2_inode_data_t* data);

	virtual ~Ext2Inode() {

	}

	std::uint32_t getBlockNum(std::uint64_t offset) {
		return _data->direct_block_addr[0];
	}

	//virtual Dentry* create(Dentry*, std::string);

	virtual Dentry * lookup(Dentry* parent, std::string name) override;

	File* getFile();

	virtual file_t* doOpen() override {
		return new Ext2File(this);
	}

	ext2_inode_data_t* _data;
};
