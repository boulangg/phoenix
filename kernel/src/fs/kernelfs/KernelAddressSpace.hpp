#pragma once

#include "KernelFSInfo.hpp"
#include "KernelInode.hpp"

#include <algorithm>
#include <cstring>


class KernelAddressSpace : public BaseAddressSpace<KernelFSInfo> {
public:
	KernelAddressSpace(KernelInode* inode) : BaseAddressSpace(inode) {}

	virtual int readPage_internal(Page* p) override;
};
