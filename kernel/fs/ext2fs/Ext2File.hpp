#pragma once

#include "Ext2FSInfo.hpp"

class Ext2File : public BaseFile<Ext2FSInfo> {
public:
	Ext2File(Ext2Dentry* dentry, Ext2Inode* inode);
	Ext2File(Ext2Inode* inode);

	virtual ~Ext2File() {

	}

	//loff_t doLseek(loff_t offset, int32_t origin) override;
	//ssize_t doRead(char* buf, ssize_t count) override;
	//ssize_t doWrite(const char* buf, ssize_t count) override;
	virtual ssize_t doRead(char* buffer, size_t size, loff_t offset) override;

};
