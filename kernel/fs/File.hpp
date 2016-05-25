/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef __FILE_HPP__
#define __FILE_HPP__

#include <cstddef>
#include <cstdint>
#include <include/constant.h>
#include <mm/PhysicalAllocator.hpp>

class File {
public:
	File(uint64_t* addr, uint64_t size): kernelStartAddr(addr), size(size) {}

	int64_t llseef(int64_t, int);
	size_t read(char *, size_t, int64_t);
	size_t write(char*, size_t, int64_t);
	int flush();

	char* getKernelStartAddr() {
		return (char*)kernelStartAddr;
	}

	Page* getPage(uint64_t index) {
		uint64_t pageAddr = (uint64_t)kernelStartAddr+index*PAGE_SIZE;
		pageAddr &= ~(KERNEL_HIGH_VMA);
		return PhysicalAllocator::getPageFromAddr((uint64_t*)pageAddr);
	}


private:
	uint64_t* kernelStartAddr;
	uint64_t size;
};


#endif // __FILE_HPP__
