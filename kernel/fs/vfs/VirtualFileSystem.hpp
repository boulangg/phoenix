#pragma once

#include <cstring>
#include <cstdint>
#include <string>
#include <vector>
#include <list>

#include <dirent.h>

#include "Dentry.hpp"
#include "File.hpp"
#include "Inode.hpp"

#define EACCES -1

class FileSystemType {
public:
	virtual ~FileSystemType() {

	}

	// 3 versions
	//struct super_block *(*read_super) (struct super_block *, void *, int);
	//struct super_block *(*get_sb)(struct file_system_type *,
	//                int, char *, void *, struct vfsmount *);
	//virtual Dentry* mount(int, const char *, void *);
	//virtual SuperBlock* readSuperBlock(Dentry* source, const void* data) = 0;
	virtual SuperBlock* readSuperBlock(const std::string& source, const void* data) = 0;


protected:
	FileSystemType(std::string name) : name(name),
			fs_flags(), supers() {

	}

public:
	std::string name;
	int fs_flags;
	std::list<SuperBlock*> supers;
};

class VirtualFileSystem {
public:
	static void initVFS();

	static std::vector<std::string> parsePathname(const std::string& path);

	static int open(const std::string& pathname);

	static int close(int fd);

	static int mount(const char* source, const char* target,
			const char* fileSystemType, std::uint64_t mountFlags,
			const void* data);

	static FileSystemType* findFS(const char* filesystemtype) {
		for (auto fst : fileSystemType) {
			if (fst->name.compare(filesystemtype) == 0) {
				return fst;
			}
		}
		return nullptr;
	}

	static int getdents64(unsigned int fd, struct linux_dirent64 *dirp, size_t count) {
		return filestable[fd]->getdents64(dirp, count);
	}

	static void registerFS(FileSystemType* fs) {
		fileSystemType.push_back(fs);
	}

	static Dentry* root;
	static std::vector<File*> filestable;
	static std::list<FileSystemType*> fileSystemType;
};



/*
class VirtualFileSystem {
public:
	VirtualFileSystem();
	virtual ~VirtualFileSystem();

	// FileDesc functions
	static FileDesc open(const char* filename, int flags, int mode);
	static void close(FileDesc fd);

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
};*/

