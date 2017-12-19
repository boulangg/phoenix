#pragma once

#include <fs/vfs/File.hpp>
#include <cstdint>
#include <include/constant.h>
#include <mm/PhysicalAllocator.hpp>

#include "KernelFSInfo.hpp"
#include "KernelInode.hpp"
#include "KernelDentry.hpp"

class KernelFile : public BaseFile<KernelFSInfo> {
public:
	KernelFile(KernelInode* inode) : BaseFile(inode) {
		//_inode = inode;
		_kernelStartAddr = (char*)inode->app.apps_start;
		_size = inode->app.apps_end - inode->app.apps_start;
		_pos = 0;
	}

	virtual ssize_t read_internal(char* ptr, size_t count, loff_t offset) override {
		/* TODO use PageCache and AddressSpace instead of _kernelStartAddr
		AddressSpace* mapping = _inode->mapping;
		int fileIndex = offset/PAGE_SIZE;
		Page* filePage = PageCache::getPage(mapping, fileIndex);
		mapping->readPage(filePage);
		*/

		char* out = (char*)ptr;
		size_t x = 0;
		for (; x < count; ++x) {
			if (offset >= (int64_t)_size) {
				break;
			}
			out[x] = _kernelStartAddr[offset];
			++offset;
		}
		return x;
	}

private:
	char* _kernelStartAddr;
};
