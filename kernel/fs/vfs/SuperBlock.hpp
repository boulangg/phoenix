#pragma once

#include <cstdint>
#include <cstddef>

#include <string>
#include <list>
#include <vector>

#include <unistd.h>
#include <time.h>

#include <mm/Page.hpp>


//typedef std::uint16_t umode_t;
//typedef std::uint32_t isolate_mode_t;

//struct BlockDevice;

/* Syscalls:
(000) read			int sys_read(unsigned int fd, char *buf, size_t count)
(001) write			int sys_write(unsigned int fd, char __user *buf, size_t count, loff_t pos
(002) open
(003) close
(004) stat
(005) fstat
(006) lstat
(007) poll
(008) lseek
(016) ioctl
(017) pread64
(018) pwrite64
(019) readv
(020) writev
(021) access
(022) pipe
(023) select
(032) dup
(033) dup2
(040) sendfile
(072) fcntl
(073) flock
(074) fsync
(075) fdatasync
(076) truncate
(077) ftruncate
(078) getdents
(079) getcwd
(080) chdir
(081) fchdir
(082) rename
(083) mkdir
(084) rmdir
(085) creat
(086) link
(087) unlink
(088) symlink
(089) readlink
(090) chmod
(091) fchmod
(092) chown
(093) fchown
(094) lchown
(095) umask
(133) mkmod
(161) chroot
(165) mount
(166) umount2
(167) swapon
(168) swapoff
(187) readahead
(217) getdents64
(221) fadvise64
(257) openat
(258) mkdirat
(259) mknodat
(260) fchownat
(261) futimesat
(262) newfstatat
(263) unlinkat
(264) renameat
(265) linkat
(266) symlinkat
(267) fchmodat
(268) faccessat
(269) pselect6
(270) ppoll
*/

#include "Dentry.hpp"
#include "AddressSpace.hpp"
#include "Inode.hpp"
#include "DentryCache.hpp"
#include "File.hpp"

struct BlockDevice;

/*#include <fs/File.hpp>

class TestFile : public File {
public:
	TestFile(NewFile* n) {
		this->n = n;
	}
	virtual ~TestFile() {

	}

	virtual size_t read(void* ptr, size_t count) {
		return n->read((char*)ptr, count, 0);
	}

	NewFile* n;
};*/

// Supporting classes


/*class vfsmount {
public:

private:
	vfsmount* parent;
	Dentry* mountpoint;
	Dentry* root;
	SuperBlock* sb;
	int mntFlags;
	std::string devName;
};

class fsStruct {
public:

private:
	Dentry* root, pwd;
	vfsmount* rootmnt, pwdmnt;
};

class nameidata {
public:

private:
	Dentry* dentry;
	vfsmount* mnt;
	std::string last;
	unsigned int flags;
	int last_type;
};*/


// Main classes

class FileSystemType;

class SuperBlock {
public:
	SuperBlock(FileSystemType* type);
	SuperBlock(FileSystemType* type, Dentry* root);
	virtual ~SuperBlock(); // put_super

	// super_operations
	virtual Inode* allocInode() = 0;
	//virtual void destroyInode(Inode*); delete inode;
	//virtual int sync(int wait);
	//virtual int freeze();
	//virtual int unfreeze();
	//virtual int remount(int *, char *);
	//virtual void umount_begin ();
	//ssize_t (*quota_read)(struct super_block *, int, char *, size_t, loff_t);
	//ssize_t (*quota_write)(struct super_block *, int, const char *, size_t, loff_t);
	//int (*nr_cached_objects)(struct super_block *);
	//void (*free_cached_objects)(struct super_block *, int);

protected:
	SuperBlock();

public:
	BlockDevice* dev;
	std::uint64_t blockSize; // For the filesystem, not the eventual device
	FileSystemType* type;
	Dentry* root;
	//std::list<File*> openFiles;
	//std::list<Inode*> dirtyInodes;
};

/*class File {
public:
	virtual ~File();

	// file_operation
	virtual loff_t lseek(loff_t, int32_t);
	virtual ssize_t read(char*, size_t, loff_t);
	virtual ssize_t write(const char*, size_t, loff_t);
	//ssize_t (*read_iter) (struct kiocb *, struct iov_iter *);
	//ssize_t (*write_iter) (struct kiocb *, struct iov_iter *);
	virtual int iterate(struct dir_context *); //readdir
	virtual uint32_t poll(struct poll_table_struct *);
	long ioctl(unsigned int, unsigned long);
	//long (*compat_ioctl) (struct file *, unsigned int, unsigned long);
	virtual int mmap(struct vm_area_struct *);
	virtual int mremap(struct file *, struct vm_area_struct *);
	virtual int flush(void* id);
	//int fsync(loff_t, loff_t, int datasync);
	//int fasync(int, int);
	//int lock(int, struct file_lock *);
	//ssize_t sendpage(struct page *, int, size_t, loff_t *, int);
	virtual unsigned long get_unmapped_area(unsigned long, unsigned long, unsigned long, unsigned long);

	virtual int flock(int, struct file_lock *);
	virtual ssize_t splice_write(struct pipe_inode_info *, loff_t *, size_t, unsigned int);
	virtual ssize_t splice_read(loff_t *, struct pipe_inode_info *, size_t, unsigned int);
	virtual int setlease(long, struct file_lock **, void **);
	virtual long fallocate(int mode, loff_t offset, loff_t len);
	virtual void show_fdinfo(struct seq_file *m);
	virtual unsigned mmap_capabilities();



protected:
	File();

private:
	Dentry* dentry;
	mode_t mode;
	loff_t pos;
	std::uint32_t uid;
	std::uint32_t gid;
	std::uint64_t count;

};*/



// super_operation
//virtual int show_options(struct seq_file *, struct dentry *);

// inode_operation

// adress_space_operation


// file_operation
//virtual int release(struct inode *, struct file *);
//virtual int check_flags(int);

// dentry_operation

/*class DentryCache {

	dget: open a new handle for an existing dentry (this just increments
			the usage count)

	dput: close a handle for a dentry (decrements the usage count). If
	the usage count drops to 0, and the dentry is still in its
	parent's hash, the "d_delete" method is called to check whether
	it should be cached. If it should not be cached, or if the dentry
	is not hashed, it is deleted. Otherwise cached dentries are put
	into an LRU list to be reclaimed on memory shortage.

	d_drop: this unhashes a dentry from its parents hash list. A
	subsequent call to dput() will deallocate the dentry if its
	usage count drops to 0

	d_delete: delete a dentry. If there are no other open references to
	the dentry then the dentry is turned into a negative dentry
	(the d_iput() method is called). If there are other
	references, then d_drop() is called instead

	d_add: add a dentry to its parents hash list and then calls
	d_instantiate()

	d_instantiate: add a dentry to the alias hash list for the inode and
	updates the "d_inode" member. The "i_count" member in the
	inode structure should be set/incremented. If the inode
	pointer is NULL, the dentry is called a "negative
	dentry". This function is commonly called when an inode is
	created for an existing negative dentry

	d_lookup: look up a dentry given its parent and path name component
	It looks up the child of that given name from the dcache
	hash table. If it is found, the reference count is incremented
	and the dentry is returned. The caller must use dput()
	to free the dentry when it finishes using it.
};*/


