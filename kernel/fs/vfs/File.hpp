#pragma once

#include "Dentry.hpp"
#include "Inode.hpp"

#include <unistd.h>

#include <sys/types.h>
#include <cstdint>
typedef int64_t ssize_t;

class Page;

class File {
public:
	File() {}
	File(Inode* inode) : ttyinode(inode) {} // for TTY
	virtual ~File();

	// file_operation
	//virtual loff_t lseek(loff_t, int32_t);
	ssize_t read(char* buffer, size_t size) {
		ssize_t bytes = doRead(buffer, size, _pos);
		_pos += bytes;
		return bytes;
	}


	ssize_t write(char* buffer, size_t size) {
		ssize_t bytes = doWrite(buffer, size, _pos);
		_pos += bytes;
		return bytes;
	}

	int64_t lseek(int64_t offset, uint32_t origin) {
		return doLseek(offset, origin);
	}

	/*int mmap(struct VirtualArea *area) {
		return doMmap(area);
	}*/

	virtual Dentry* getDentry() {
		return nullptr;
	}

	virtual Inode* getInode() {
		return ttyinode;
	}

	virtual loff_t getPos() {
		return 0;
	}

	virtual int64_t doLseek(int64_t offset, uint32_t origin) {
		(void)offset; (void)origin;
		return 0;
	}

	/*virtual int doMmap(struct VirtualArea *area) {
		return 0;
	}*/

private:
	virtual ssize_t doRead(char* buffer, size_t size, loff_t offset) = 0;
	virtual ssize_t doWrite(char* buffer, size_t size, loff_t offset);
	//virtual ssize_t write(const char*, size_t, loff_t);
	//ssize_t (*read_iter) (struct kiocb *, struct iov_iter *);
	//ssize_t (*write_iter) (struct kiocb *, struct iov_iter *);
	//virtual int iterate(struct dir_context *); //readdir
	//virtual uint32_t poll(struct poll_table_struct *);
	//long ioctl(unsigned int, unsigned long);
	//long (*compat_ioctl) (struct NewFile *, unsigned int, unsigned long);
	//virtual int mremap(struct NewFile *, struct vm_area_struct *);
	//virtual int flush(void* id);
	//int fsync(loff_t, loff_t, int datasync);
	//int fasync(int, int);
	//int lock(int, struct NewFile_lock *);
	//ssize_t sendpage(struct page *, int, size_t, loff_t *, int);
	//virtual unsigned long get_unmapped_area(unsigned long, unsigned long, unsigned long, unsigned long);

	//virtual int flock(int, struct NewFile_lock *);
	//virtual ssize_t splice_write(struct pipe_inode_info *, loff_t *, size_t, unsigned int);
	//virtual ssize_t splice_read(loff_t *, struct pipe_inode_info *, size_t, unsigned int);
	//virtual int setlease(long, struct NewFile_lock **, void **);
	//virtual long fallocate(int mode, loff_t offset, loff_t len);
	//virtual void show_fdinfo(struct seq_file *m);
	//virtual unsigned mmap_capabilities();



//protected:
	File(Dentry* d);

//private:
public:
	//Dentry* dentry;
	Inode* ttyinode;
	loff_t _pos;
	//mode_t mode;
	//loff_t pos;
	//std::uint32_t uid;
	//std::uint32_t gid;
	std::uint64_t count;

};

/*#ifndef __FILE_HPP__
#define __FILE_HPP__

#include <cstddef>
#include <cstdint>
#include <unistd.h>

class File {
protected:
	File();
	//File(Inode); // open
	virtual ~File(); // release

public:

	opendir: (user side) open
	closedir: (user side) close
	rewinddir: (user side) seekdir to beginning
	scandir: (user side) getdents
	seekdir: (user side) lseek



	// Kernel functionalities
	virtual Page* getPage(uint64_t index);

private:
	virtual loff_t do_llseek(loff_t, int32_t);
	virtual ssize_t do_read(char*, size_t, loff_t*);
	virtual ssize_t do_write(const char*, size_t, loff_t*);
	virtual uint32_t do_poll(uint32_t, uint64_t);
	virtual int32_t do_flush();
};


#endif // __FILE_HPP__ */
