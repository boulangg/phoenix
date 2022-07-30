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
	//SuperBlock(FileSystemType* type, Dentry* root);
	virtual ~SuperBlock(); // put_super

	// super_operations
	virtual Inode* allocInode() = 0;
	virtual Dentry* getRoot() = 0;
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
	//std::uint64_t blockSize; // For the filesystem, not the eventual device
	FileSystemType* type;
	//Dentry* root;
	//std::list<File*> openFiles;
	//std::list<Inode*> dirtyInodes;
};

/// VFS orphan methods

// super_operation
//virtual int show_options(struct seq_file *, struct dentry *);

// inode_operation

// adress_space_operation

// file_operation
//virtual int release(struct inode *, struct file *);
//virtual int check_flags(int);

// dentry_operation




