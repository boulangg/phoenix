/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include "dev/BlockCache.h"
#include "dev/BlockDevice.h"

#include "fs/SuperBlock.h"

#include "Ext2Inode.h"
#include "Ext2Struct.h"

namespace kernel::fs::ext2fs {

class Ext2SuperBlock : public SuperBlock
{
public:
    Ext2SuperBlock(dev::BlockDevice* dev);
    ~Ext2SuperBlock();

    Inode* getInode(std::size_t inodeNum);

    std::uint64_t getBlockSize()
    {
        return 1ull << _logBlockShift;
    }

    void readBlocks(mem::Page* p, std::vector<std::uint64_t> blkNums);

private:
    ext2_block_group_desc readBlockGroupDesc(std::uint64_t blockGrounNum);
    ext2_inode_data readInodeData(std::uint64_t inodeNum);

    dev::BlockDevice* _blkDev;
    ext2_superblock _sb;
    dev::BlockCache _blkCache;

    ext2_block_group_desc* _blockGroupDescTable;
    std::uint64_t _blockGroupCount;
    std::uint32_t _logBlockShift;
    std::uint32_t _inodeStructSize;
};

}
