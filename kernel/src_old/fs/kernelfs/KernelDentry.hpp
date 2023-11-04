#pragma once

#include "KernelFSInfo.hpp"

#include "KernelFile.hpp"

class KernelDentry : public BaseDentry<KernelFSInfo>
{
public:
	KernelDentry(KernelInode* inode) : BaseDentry(inode)
	{

	}

	KernelDentry(Dentry* parent, KernelInode* inode, std::string name) : BaseDentry(parent, inode, name)
	{

	}
};
