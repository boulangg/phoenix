#include "Ext2Inode.hpp"

#include "Ext2SuperBlock.hpp"
#include "Ext2AddressSpace.hpp"

#include <mm/PhysicalAllocator.hpp>

Ext2Inode::Ext2Inode(Ext2SuperBlock* sb, std::uint64_t ino, ext2_inode_data_t* data) :
		BaseInode(sb, ino, data->file_size_low), _data(data) {
		mapping = new Ext2AddressSpace(this);
	}

Dentry * Ext2Inode::lookup(Dentry* parent, std::string name) {
	if (_data->type_n_perm & ext2_type_n_perm_t::DIRECTORY) {
		Page* p = PhysicalAllocator::allocPage();
		char* buffer = (char*)p->kernelMappAddr;
		Ext2File* file = open_internal();
		while (true) {
			// Read directory entry header:
			ext2_directory_entry_header_t header;
			if (file->read((char*)&header, sizeof(ext2_directory_entry_header_t)) == 0) {
				break;
			}
			// Read file name:
			file->read(buffer, header.entry_size - sizeof(ext2_directory_entry_header_t));
			std::string subDirName(buffer);
			if (subDirName.compare(name) == 0) {
				// file found
				Ext2Inode* inode = sb->getInode(header.inode);
				Ext2Dentry* dentry = new Ext2Dentry(parent, inode, name);
				return dentry;
			}
		}
	}
	return nullptr;
}

std::uint32_t Ext2Inode::getBlockNum(std::uint64_t offset) {
	// TODO check offset < file size
	return _data->direct_block_addr[offset/sb->getBlockSize()];
}
