/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #include "Ext2Inode.h"
#include "Ext2SuperBlock.h"

namespace kernel::fs::ext2fs {

static std::uint64_t _getFileSize(ext2_inode_data inodeData)
{
    return uint64_t(inodeData.file_size_high) << 32 | inodeData.file_size_low;
}

Ext2Inode::Ext2Inode(Ext2SuperBlock* sb, ext2_inode_data inodeData) :
    Inode(_getFileSize(inodeData)), _sb(sb), _inodeData(inodeData)
{}

Ext2Inode::~Ext2Inode() {}

Inode* Ext2Inode::lookup(const std::string& pathNameComponent)
{
    std::size_t offset = 0;
    std::size_t count = 0;
    char buffer[256];
    while (true) {
        // Read directory entry header:
        ext2_directory_entry_header header{};
        count = this->read((char*)&header, sizeof(ext2_directory_entry_header), offset);
        if (count == 0) {
            break;
        }
        // Read file name:
        count = this->read(buffer, header.name_length, offset);
        offset += header.entry_size;
        std::string subDirName(buffer, header.name_length);
        if (subDirName.compare(pathNameComponent) == 0) {
            // file found
            return _sb->getInode(header.inode);
        }
    }

    return nullptr;
}

void Ext2Inode::readpage(mem::Page* p, std::size_t pageNum)
{
    // TODO: use FileAddressSpace1
    size_t logBlockPerPage = PAGE_SIZE / _sb->getBlockSize();
    size_t logBlockNumStart = pageNum * logBlockPerPage;
    size_t logBlockNumEnd = (pageNum + 1) * logBlockPerPage;
    std::vector<std::uint64_t> blkNums;
    for (size_t logBlockNum = logBlockNumStart; logBlockNum < logBlockNumEnd; ++logBlockNum) {
        auto blkAddr = (_inodeData.direct_block_addr[logBlockNum]);
        if (blkAddr == 0) {
            break;
        }
        blkNums.push_back(blkAddr);
    }
    _sb->readBlocks(p, blkNums);
}

}
