/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef __VIRTUALFILESYSTEM_HPP__
#define __VIRTUALFILESYSTEM_HPP__

#include <fs/vfs/Inode.hpp>
#include <fs/vfs/Superblock.hpp>

struct GlobalOpenFile {
	uint32_t refCount;
	uint8_t openMode;
	Inode* inode;
};

struct LocalOpenFile {
	int64_t openFileTableIndex;
	uint64_t offset;
};

class VirtualFileSystem {
public:
	VirtualFileSystem();
	virtual ~VirtualFileSystem();

	// FileDesc functions
	static FileDesc open(const char* filename, int flags, int mode);
	static void close(FileDesc fd);
	static loff_t lseek(FileDesc fd, loff_t offset, int32_t origin);
	static ssize_t read(FileDesc fd, char* buf, ssize_t count);
	static ssize_t write(FileDesc fd, const char* buf, ssize_t count);

	static FileDesc dup(FileDesc);

	// link, unlink, chown, chmod, stat, lstat
	// readlink, symlink
	// mkdir, rmdir, getdents, fstat

	// Misc functions
	static int mount(const char* source, const char* target,
			const char* fileSystemType, uint64_t mountFlags,
			const void* data);
	static int umount(const char* target);
	static int rename(const char* oldPath, const char* newPath);

	// Register file system
	static int registerFileSystem(FileSystem* fs);

private:
	//static Inode* getInode(uint64_t inodeNumber);

	static Dentry* findDentry(const char* filename, Dentry* workDir) {
		Dentry* start = workDir;
		if (filename[0] = '/') {
			start = root;
		}
		std::list<char*> path;
		while ()
	}

	static std::vector<File*> _openFileTable;
	static std::list<Inode*> _inodeTable;
	static Dentry* root;

	static std::list<FileSystem*> _fileSystems;
};

#endif // __VIRTUALFILESYSTEM_HPP__
