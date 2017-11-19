/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef __INODE_HPP__
#define __INODE_HPP__

#include <fs/vfs/File.hpp>

struct FileDesc {
	int64_t _fileIndex;
};

class File {
	Inode* _inode;
	int8_t _mode;
	uint64_t _offset;
	uint64_t _refCount;
};

class Inode {
public:
	Inode();
	virtual ~Inode();



	// FILE
	loff_t lseek(loff_t, int32_t);
	ssize_t read(char*, size_t, loff_t*);
	ssize_t write(const char*, size_t, loff_t*);
	uint32_t poll(uint32_t, uint64_t);
	int32_t flush();

	// DIR
	int32_t getdents(struct dirent*, uint32_t);

	int mount(Inode*);
	int unmount();

private:
	uint32_t _refCount;
	uint64_t _inodeNumber;

	Dentry* _path;

	bool _mount;
	Inode* _mountInode;
};

struct Dentry {
	Inode* inode;
	Dentry* parent;
	char* name;
	std::list<Dentry*> children;

};

// Process: std::vector<FileDesc> _fileDescTable
// FileDesc (int64_t fileIndex, ...)
// FileDesc (static): NullFileDesc
// VirtualFileSystem (static): std::vector<File*> _openFileTable;
// File (Inode*, mode, offset, compteur, ...)
// VirtualFileSystem (static): std::list<Inode*> _inodeTable;
// Inode (compteur, inodeNum, InodeInfos, device, Dentry*, filesystems?, blocTable)
// VirtualFileSystem (static): Dentry* _;
// Dentry (name, parent, children, inode)

#endif // __INODE_HPP__
