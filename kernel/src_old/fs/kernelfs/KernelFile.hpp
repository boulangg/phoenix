#pragma once

#include <fs/vfs/File.hpp>
#include <cstdint>
#include <string.h>
#include <algorithm>
#include <include/constant.h>
#include <mm/PhysicalAllocator.hpp>

#include "KernelFSInfo.hpp"
#include "KernelInode.hpp"
#include "KernelDentry.hpp"

class KernelFile : public BaseFile<KernelFSInfo>
{
public:
	KernelFile(KernelInode* inode) : BaseFile(inode)
	{
		_kernelStartAddr = (char*)inode->app.apps_start;
		_pos = 0;
	}

	virtual ssize_t read_internal(char* buffer, size_t size, loff_t offset) override
	{
		std::uint64_t i;
		for (i = offset; i < offset + size && i < _inode->size; ) {
			size_t pageNo = i / PAGE_SIZE;
			Page* p = _inode->mapping->getPage(pageNo);
			_inode->mapping->readPage(p);
			void* destination = buffer + i - offset;
			void* source = ((char*)p->kernelMappAddr) + i % PAGE_SIZE;
			std::uint64_t length = std::min<std::uint64_t>(_inode->size - i, offset + size - i);
			length = std::min(length, (pageNo + 1) * PAGE_SIZE - i);
			memcpy(destination, source, length);
			i += length;
		}
		return i - offset;
	}

private:
	char* _kernelStartAddr;
};
