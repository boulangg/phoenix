/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include <fs/KernelFile.hpp>

KernelFile::KernelFile(uint64_t* addr, uint64_t size) :
		_kernelStartAddr((char*)addr), _size(size), _offset(0) {

}

KernelFile::~KernelFile() {

}

int64_t KernelFile::lseek(int64_t offset, uint32_t origin) {
	// TODO check overflows
	switch(origin) {
	case SEEK_SET:
		_offset = offset;
		break;
	case SEEK_CUR:
		_offset += offset;
		break;
	case SEEK_END:
		_offset = _size + offset;
		break;
	default:
		return -1;
	}

	if (_offset > (int64_t)_size) {
		_offset = _size;
	}
	if (_offset < 0) {
		_offset = 0;
	}
	return 0;
}

size_t KernelFile::read(void* ptr, size_t count) {
	char* out = (char*)ptr;
	size_t x = 0;
	for (; x < count; ++x) {
		if (_offset >= (int64_t)_size) {
			break;
		}
		out[x] = _kernelStartAddr[_offset];
		++_offset;
	}
	return x;
}
