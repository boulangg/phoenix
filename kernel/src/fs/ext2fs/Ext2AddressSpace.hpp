#pragma once

#include "Ext2FSInfo.hpp"
#include "Ext2Inode.hpp"

#include <algorithm>
#include <cstring>

#include "Ext2SuperBlock.hpp"

class Ext2AddressSpace : public BaseAddressSpace<Ext2FSInfo>
{
public:
	Ext2AddressSpace(Ext2Inode* inode) : BaseAddressSpace(inode)
	{}

	virtual int readPage_internal(Page* p) override;
};
