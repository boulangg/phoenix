/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #include "Ext2SuperBlock.h"

#include <cstring>
#include <stdexcept>

#include "KernelGlobals.h"
#include "utils/Utils.h"

namespace kernel::fs::ext2fs {

constexpr std::uint64_t DEFAULT_BLOCK_SHIFT = 10;
constexpr std::uint64_t DEFAULT_BLOCK_SIZE = 1 << DEFAULT_BLOCK_SHIFT;

Ext2SuperBlock::Ext2SuperBlock(dev::BlockDevice* blkDev) :
    SuperBlock(), _blkDev(blkDev), _blkCache(blkDev, DEFAULT_BLOCK_SHIFT)
{
    auto blk = _blkCache.getBlock(1);
    std::memcpy((void*)(&_sb), (void*)(blk.getKernelAddr()), sizeof(ext2_superblock));

    if (_sb.signature != EXT2_SIGNATURE) {
        throw std::invalid_argument("Not a EXT2 filesystem");
    }

    printk("Initialize ext2 filesystem");

    _logBlockShift = 10 + _sb.log_block_size;
    if (_logBlockShift != DEFAULT_BLOCK_SHIFT) {
        _blkCache = dev::BlockCache{_blkDev, _logBlockShift};
    }

    _blockGroupCount = utils::divRoundUp(_sb.block_group_blocks, _sb.nb_blocks);

    if (_sb.version_major < 1) {
        _inodeStructSize = 128;
    } else {
        _inodeStructSize = _sb.size_of_inode_struct;
    }

    _rootInode = getInode(2);
}

Ext2SuperBlock ::~Ext2SuperBlock() {}

Inode* Ext2SuperBlock::getInode(std::size_t inodeNum)
{
    return new Ext2Inode(this, readInodeData(inodeNum));
}

void Ext2SuperBlock::readBlocks(mem::Page* p, std::vector<std::uint64_t> blkNums)
{
    _blkDev->readBlocks(p, blkNums);
}

ext2_block_group_desc Ext2SuperBlock::readBlockGroupDesc(std::uint64_t blockGroupNum)
{
    std::uint64_t blockStart = 1;
    if (getBlockSize() == DEFAULT_BLOCK_SIZE) {
        blockStart = 2;
    }

    auto blockNum = blockStart + (blockGroupNum * sizeof(ext2_block_group_desc) / getBlockSize());
    auto blockOffset = (blockGroupNum * sizeof(ext2_block_group_desc) % getBlockSize());

    auto blk = _blkCache.getBlock(blockNum);
    return *reinterpret_cast<ext2_block_group_desc*>(blk.getKernelAddr() + blockOffset);
}

ext2_inode_data Ext2SuperBlock::readInodeData(std::uint64_t inodeNum)
{
    std::uint64_t blockGroupNum = (inodeNum - 1) / _sb.block_group_inodes;
    ext2_block_group_desc blockGroupDesc = readBlockGroupDesc(blockGroupNum);
    std::uint64_t blockGroupOffset = (inodeNum - 1) % _sb.block_group_inodes;
    std::uint64_t blockNum = blockGroupDesc.start_inode_table + (blockGroupOffset * _inodeStructSize) / getBlockSize();
    std::uint64_t blockOffset = (blockGroupOffset * _inodeStructSize) % getBlockSize();

    auto blk = _blkCache.getBlock(blockNum);
    return *reinterpret_cast<ext2_inode_data*>(blk.getKernelAddr() + blockOffset);
}

}
