#include "Ext2File.hpp"

#include "Ext2Inode.hpp"

#include "algorithm"
#include "cstring"

Ext2File::Ext2File(Ext2Dentry* dentry, Ext2Inode* inode) : BaseFile(dentry, inode, inode->_data->file_size_low) {

}

Ext2File::Ext2File(Ext2Inode* inode) : BaseFile(inode) {

}

ssize_t Ext2File::doRead(char* buffer, size_t size, loff_t offset) {
	std::uint64_t i;
	for (i = offset; i < offset + size && i < _inode->size; ) {
		size_t pageNo = i / PAGE_SIZE;
		Page* p = _inode->mapping->getPage(pageNo);
		_inode->mapping->doReadPage(p);
		void* destination = buffer + i - offset;
		void* source = ((char*)p->kernelMappAddr) + i % PAGE_SIZE;
		std::uint64_t length = std::min(_inode->size - i, offset + size -i);
		length = std::min(length, (pageNo + 1) * PAGE_SIZE - i);
		memcpy(destination, source, length);
		i+=length;
		/*while (i < _inode->size && i < offset + size && i < (pageNo + 1) * PAGE_SIZE) {
			buffer[i - offset] = ((char*)p->kernelMappAddr)[i%PAGE_SIZE];
			i++;
		}*/
		//i--;
	}
	return i-offset;
}
