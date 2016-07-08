/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include <fs/File.hpp>

File::File() {}

File::~File() {}

int64_t File::lseek(int64_t, uint32_t) {
	return 0;
}

size_t File::read(void*, size_t, size_t) {
	return 0;
}

size_t File::write(void*, size_t, size_t) {
	return 0;
}

int File::flush() {
	return 0;
}


