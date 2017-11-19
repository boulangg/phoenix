/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef __FILE_HPP__
#define __FILE_HPP__

#include <cstddef>
#include <cstdint>
#include <unistd.h>

class File {
protected:
	File();
	File(Inode); // open
	virtual ~File(); // release

public:
	/*
	opendir: (user side) open
	closedir: (user side) close
	rewinddir: (user side) seekdir to beginning
	scandir: (user side) getdents
	seekdir: (user side) lseek
	*/


	// Kernel functionalities
	virtual Page* getPage(uint64_t index);

private:
	/*virtual loff_t do_llseek(loff_t, int32_t);
	virtual ssize_t do_read(char*, size_t, loff_t*);
	virtual ssize_t do_write(const char*, size_t, loff_t*);
	virtual uint32_t do_poll(uint32_t, uint64_t);
	virtual int32_t do_flush();*/
};


#endif // __FILE_HPP__
