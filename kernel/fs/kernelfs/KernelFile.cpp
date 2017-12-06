/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "KernelFile.hpp"

/*KernelFile::KernelFile(uint64_t* addr, uint64_t size) :
		_kernelStartAddr((char*)addr), _size(size), _offset(0) {

}*/

/*KernelFile::~KernelFile() {

}*/

/*int64_t KernelFile::lseek(int64_t offset, uint32_t origin) {
	// TODO check overflows
	switch(origin) {
	case SEEK_SET:
		pos = offset;
		break;
	case SEEK_CUR:
		pos += offset;
		break;
	case SEEK_END:
		pos = _size + offset;
		break;
	default:
		return -1;
	}

	if (pos > (int64_t)_size) {
		pos = _size;
	}
	if (pos < 0) {
		pos = 0;
	}
	return 0;
}*/

/*ssize_t KernelFile::doRead(char* ptr, size_t count, loff_t offset) {
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
}*/
