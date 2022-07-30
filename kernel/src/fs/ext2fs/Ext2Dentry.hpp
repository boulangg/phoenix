#pragma once

#include "Ext2FSInfo.hpp"

#include "Ext2File.hpp"

class Ext2Dentry : public BaseDentry<Ext2FSInfo> {
public:
	Ext2Dentry(Ext2Inode* inode) : BaseDentry(inode) {

	}

	Ext2Dentry(Dentry* parent, Ext2Inode* inode, std::string name) : BaseDentry(parent, inode, name) {

	}
};
