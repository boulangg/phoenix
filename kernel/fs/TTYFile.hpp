/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef __FS_TTYFILE_HPP__
#define __FS_TTYFILE_HPP__

#include <fs/File.hpp>

class TTYFile: public File {
public:
	TTYFile();
	virtual ~TTYFile();

	virtual size_t read(void* ptr, size_t count);
	virtual size_t write(void* ptr, size_t count);

private:
};

#endif // __FS_TTYFILE_HPP__
