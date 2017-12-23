#include "VirtualFileSystem.hpp"

#include "DentryCache.hpp"
#include "SuperBlock.hpp"

#include "fs/kernelfs/KernelFileSystemType.hpp"
#include "fs/ext2fs/Ext2FileSystemType.hpp"

Dentry* VirtualFileSystem::root;
std::vector<File*> VirtualFileSystem::filestable;
std::list<FileSystemType*> VirtualFileSystem::fileSystemType;

void VirtualFileSystem::initVFS() {
	// Create root inode
	Inode* inode = new Inode(nullptr, 0, 0);
	root = new Dentry(inode);
	FileSystemType* fs;
	fs = new KernelFileSystemType();
	registerFS(fs);
	fs = new Ext2FileSystemType();
	registerFS(fs);
}

std::vector<std::string> VirtualFileSystem::parsePathname(const std::string& pathname) {
	// TODO correct error (doesn't handle correctly last '/' even for files)
	std::vector<std::string> res;
	size_t i = 0;
	if (pathname[0] == '/') {
		i = 1;
	}
	size_t start = i;
	for (; i < pathname.size(); i++) {
		if (pathname[i] != '/') {
			continue;
		}
		res.push_back(pathname.substr(start, i - start));
		start = i+1;
	}
	if (i != start) {
		res.push_back(pathname.substr(start, i - start));
	}

	return res;
}


int VirtualFileSystem::open(const std::string& pathname) {
	// TODO handle flags and check access right
	std::vector<std::string> pathnameVector = VirtualFileSystem::parsePathname(pathname);
	Dentry* dentry = DentryCache::findDentry(root, pathnameVector, 0);

	// File not found
	if (dentry == nullptr) {
		return EACCES;
	}

	// Check if file already open
	/*for (size_t i = 0; i < filestable.size(); i++) {
		if ((filestable[i] != nullptr)
				&& (filestable[i]->dentry->inode == dentry->inode)) {
			return i;
		}
	}*/

	File* file = dentry->getInode()->open();
	file->setDentry(dentry);
	// Check for available file descriptor
	for (size_t i = 0; i < filestable.size(); i++) {
		if (filestable[i] == nullptr) {
			filestable[i] = file;
			return i;
		}
	}

	// Create new file descriptor
	filestable.push_back(file);
	return filestable.size() - 1;
}

int VirtualFileSystem::close(int fd) {
	filestable[fd]->count--;
	if (filestable[fd]->count == 0) {
		delete filestable[fd];
	}
	filestable[fd] = nullptr;
	return 0;
}


int VirtualFileSystem::stat(const char* pathname, struct stat* stat) {
	std::vector<std::string> pathnameVector = VirtualFileSystem::parsePathname(pathname);
	Dentry* dentry = DentryCache::findDentry(root, pathnameVector, 0);
	dentry->inode->stat(stat);
	return 0;
}

int VirtualFileSystem::fstat(File* file, struct stat* stat) {
	return file->getInode()->stat(stat);
}

int VirtualFileSystem::lstat(const char* pathname, struct stat* stat) {
	std::vector<std::string> pathnameVector = VirtualFileSystem::parsePathname(pathname);
	Dentry* dentry = DentryCache::findDentry(root, pathnameVector, 0);
	return dentry->inode->stat(stat);
}

int VirtualFileSystem::mount(const char* source, const char* target,
		const char* fileSystemType, uint64_t,
		const void* data) {

	std::vector<std::string> mountVector = parsePathname(target);
	Dentry* mount = DentryCache::findDentry(VirtualFileSystem::root, mountVector, 0);
	if (mount == nullptr) {
		return 1;
	}

	FileSystemType* type = VirtualFileSystem::findFS(fileSystemType);

	SuperBlock* sb = type->readSuperBlock(source, data);
	if (sb) {
		mount->mount = sb->getRoot();
		return 0;
	}
	return 1;
}


