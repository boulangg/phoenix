/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef __FS_KERNELFILE_HPP__
#define __FS_KERNELFILE_HPP__

#include <fs/File.hpp>

class KernelFile: public File {
public:
	KernelFile(uint64_t* addr, uint64_t size);
	virtual ~KernelFile();

	virtual int64_t lseek(int64_t offset, uint32_t origin);
	virtual size_t read(void* ptr, size_t size, size_t count);

	// Kernel functionalities
	virtual Page* getPage(uint64_t index) {
		uint64_t pageAddr = (uint64_t)_kernelStartAddr+index*PAGE_SIZE;
		pageAddr &= ~(KERNEL_HIGH_VMA);
		return PhysicalAllocator::getPageFromAddr((uint64_t*)pageAddr);
	}


private:
	char* _kernelStartAddr;
	size_t _size;
	int64_t _offset;
};

#endif
