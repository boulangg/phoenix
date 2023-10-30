#pragma once

#include "Ext2FSInfo.hpp"
#include <fs/vfs/VirtualFileSystem.hpp>

class Ext2File : public BaseFile<Ext2FSInfo>
{
public:
	//Ext2File(Ext2Dentry* dentry, Ext2Inode* inode);
	Ext2File(Ext2Inode* inode);

	virtual ~Ext2File()
	{

	}

	virtual ssize_t read_internal(char* buffer, size_t size, loff_t offset) override;
	virtual int getdents64_internal(struct linux_dirent64* dirp, size_t size) override;

};
