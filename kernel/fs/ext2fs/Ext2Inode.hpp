
#pragma once

#include <fs/ext2fs/Ext2FSInfo.hpp>

#include "Ext2File.hpp"

class Ext2Inode : public BaseInode<Ext2FSInfo> {
public:
	Ext2Inode(Ext2SuperBlock* sb, std::uint64_t ino, ext2_inode_data_t* data);

	virtual ~Ext2Inode() {

	}

	std::uint32_t getBlockNum(std::uint64_t offset);

	virtual Dentry * lookup(Dentry* parent, std::string name) override;

	File* getFile();

	virtual file_t* open_internal() override {
		return new Ext2File(this);
	}

	virtual int stat_internal(struct stat* stat) override;

	ext2_inode_data_t* _data;
};
