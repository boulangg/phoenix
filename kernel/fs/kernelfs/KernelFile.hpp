#pragma once

#include <fs/vfs/File.hpp>
#include <cstdint>
#include <include/constant.h>
#include <mm/PhysicalAllocator.hpp>

#include "KernelFSInfo.hpp"
#include "KernelInode.hpp"
#include "KernelDentry.hpp"

/*class KernelFile: public File {
public:
	KernelFile(uint64_t* addr, uint64_t size);
	KernelFile(Inode* inode) : File(inode) {
		KernelInode* kinode = (KernelInode*) inode;
		_kernelStartAddr = (char*)kinode->app.apps_start;
		_size = kinode->app.apps_end - kinode->app.apps_start;
		_offset = 0;
	}
	virtual ~KernelFile();

	virtual int64_t lseek(int64_t offset, uint32_t origin);
	//virtual size_t read(void* ptr, size_t count);

	// Kernel functionalities
	virtual Page* getPage(uint64_t index) {
		uint64_t pageAddr = (uint64_t)_kernelStartAddr+index*PAGE_SIZE;
		pageAddr &= ~(KERNEL_HIGH_VMA);
		return PhysicalAllocator::getPageFromAddr((uint64_t*)pageAddr);
	}

protected:
	virtual ssize_t doRead(char* buffer, size_t size, loff_t offset);

private:
	char* _kernelStartAddr;
	size_t _size;
	int64_t _offset;
};*/

class KernelFile : public BaseFile<KernelFSInfo> {
public:
	KernelFile(KernelInode* inode) : BaseFile() {
		_inode = inode;
		_kernelStartAddr = (char*)inode->app.apps_start;
		_size = inode->app.apps_end - inode->app.apps_start;
		_pos = 0;
	}

	void setDentry(KernelDentry* dentry) {
		_dentry = dentry;
	}

	virtual ssize_t doRead(char* ptr, size_t count, loff_t offset) override{
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

	virtual Page* getPage(uint64_t index) {
		uint64_t pageAddr = (uint64_t)_kernelStartAddr+index*PAGE_SIZE;
		pageAddr &= ~(KERNEL_HIGH_VMA);
		return PhysicalAllocator::getPageFromAddr((uint64_t*)pageAddr);
	}

private:
	char* _kernelStartAddr;
};
