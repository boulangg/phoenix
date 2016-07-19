/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include <fs/TTYFile.hpp>
#include <core/Console.hpp>

TTYFile::TTYFile() {}

TTYFile::~TTYFile() {}

size_t TTYFile::read(void* ptr, size_t count) {
	(void)ptr; (void)count;
	return 0;
}

size_t TTYFile::write(void* ptr, size_t count) {
	char* in = (char*)ptr;
	size_t i = 0;
	for (; i < count; ++i) {
		Console::write(in[i]);
	}
	return i;
}
