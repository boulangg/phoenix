
#include "VirtualFileSystem.hpp"

loff_t VirtualFileSystem::lseek(FileDesc fd, loff_t offset, int32_t origin) {
	File* f = _openFileTable[fd._fileIndex];
	return f->lseek(offset, origin);
}

ssize_t VirtualFileSystem::read(FileDesc fd, char* buf, ssize_t count) {
	File* f = _openFileTable[fd._fileIndex];
	return f->read(buf, count);
}


