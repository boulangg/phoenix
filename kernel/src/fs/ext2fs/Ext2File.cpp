#include "Ext2File.hpp"

#include "Ext2Inode.hpp"

#include "algorithm"
#include "cstring"

Ext2File::Ext2File(Ext2Inode* inode) : BaseFile(inode) {

}

ssize_t Ext2File::read_internal(char* buffer, size_t size, loff_t offset) {
	std::uint64_t i;
	for (i = offset; i < offset + size && i < _inode->size; ) {
		size_t pageNo = i / PAGE_SIZE;
		Page* p = _inode->mapping->getPage(pageNo);
		_inode->mapping->readPage(p);
		void* destination = buffer + i - offset;
		void* source = ((char*)p->kernelMappAddr) + i % PAGE_SIZE;
		std::uint64_t length = std::min(_inode->size - i, offset + size -i);
		length = std::min(length, (pageNo + 1) * PAGE_SIZE - i);
		memcpy(destination, source, length);
		i+=length;
	}
	return i-offset;
}

#include <core/Debug.hpp>

check_size<sizeof(struct linux_dirent64), 24> testsize;

int Ext2File::getdents64_internal(struct linux_dirent64 *dirp, size_t size) {
	int i = 0;
	while (true) {
		char buffer[4096];
		// Read directory entry header:
		ext2_directory_entry_header_t header;
		if (read((char*)&header, sizeof(ext2_directory_entry_header_t)) == 0) {
			return i;
		}
		if (i + header.name_length + sizeof(struct linux_dirent64) > size) {
			lseek(i, SEEK_SET);
			return i;
		}
		// Read file name:
		read(buffer, header.entry_size - sizeof(ext2_directory_entry_header_t));
		struct linux_dirent64* newdirp = (struct linux_dirent64*)(((char*)dirp) + i);
		newdirp->d_ino = header.inode;
		newdirp->d_reclen = header.name_length + sizeof(struct linux_dirent64);
		i += newdirp->d_reclen;
		newdirp->d_off = i;
		memcpy((char*)newdirp->d_name, buffer, header.name_length);
		newdirp->d_name[header.name_length] = '\0';
		newdirp->d_type = header.type_indicator;
	}
}
