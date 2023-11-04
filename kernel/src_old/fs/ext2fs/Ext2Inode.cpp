/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "Ext2Inode.hpp"

#include "Ext2AddressSpace.hpp"
#include "Ext2SuperBlock.hpp"

#include <mm/PhysicalAllocator.hpp>

Ext2Inode::Ext2Inode(Ext2SuperBlock* sb, std::uint64_t ino, ext2_inode_data_t* data) :
    BaseInode(sb, ino, data->file_size_low), _data(data)
{
    mapping = new Ext2AddressSpace(this);
    if (data->type_n_perm & REGULAR_FILE) {
        size = data->file_size_low | ((uint64_t)data->file_size_high << 32);
    } else {
        size = data->file_size_low;
    }
}

Dentry* Ext2Inode::lookup(Dentry* parent, std::string name)
{
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
            std::string subDirName(buffer, header.name_length);
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

std::uint32_t Ext2Inode::getBlockNum(std::uint64_t offset)
{
    // TODO check offset < file size
    return _data->direct_block_addr[offset / sb->getBlockSize()];
}

mode_t Ext2Inode::getMode()
{
    return _data->type_n_perm;
}

dev_t Ext2Inode::getDeviceID()
{
    return _data->direct_block_addr[0];
}

int Ext2Inode::stat_internal(struct stat* stat)
{
    stat->st_dev = 0; // sb->_dev->getDeviceNumber
    stat->st_ino = ino;
    stat->st_mode = _data->type_n_perm;
    stat->st_nlink = _data->nb_hard_link;
    stat->st_uid = _data->user_ID;
    stat->st_gid = _data->group_ID;
    if ((_data->type_n_perm & CHARACTER_DEVICE) || (_data->type_n_perm & BLOCK_DEVICE)) {
        stat->st_rdev = _data->direct_block_addr[0];
    }
    stat->st_size = size;
    stat->st_atime.tv_sec = _data->last_access_time;
    stat->st_mtime.tv_sec = _data->last_modif_time;
    stat->st_ctime.tv_sec = _data->last_modif_time;
    stat->st_blksize = sb->getBlockSize();
    stat->st_blocks = _data->nb_disk_sectors;
    return 0;
}
