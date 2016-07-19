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
#include <unistd.h>

class File {
protected:
	File();
	virtual ~File();

public:
	// User space functionalities
	virtual int64_t lseek(int64_t, uint32_t);
	virtual size_t read(void* ptr, size_t count);
	virtual size_t write(void* ptr, size_t count);
	virtual int flush();


	// Kernel functionalities
	virtual Page* getPage(uint64_t index);
};


#endif // __FILE_HPP__
