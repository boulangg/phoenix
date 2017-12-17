
#pragma once

#include <algorithm>
#include <cstring>

#include <fs/vfs/SuperBlock.hpp>

#include <driver/DeviceManager.hpp>
#include <mm/PhysicalAllocator.hpp>

#include "Ext2FSInfo.hpp"
#include <core/Debug.hpp>

#include "Ext2Inode.hpp"
#include "Ext2Dentry.hpp"

class Ext2SuperBlock : public SuperBlock {
public:
	Ext2SuperBlock(FileSystemType* type, Dentry* src) :
		SuperBlock(type), _logBlockSize(10)
	{
		(void)src;
		_dev = DeviceManager::getBlockDevice("initrd");
		Block* blk = _dev->getBlock(1024/_dev->getSectorSize());
		ext2_superblock_t* sb = new ext2_superblock_t();
		memcpy((void*)sb, (void*)(((char*)(blk->page->kernelMappAddr)) + blk->offset), 512);
		if (sb->signature == EXT2_SIGNATURE) {
			printk("Found ext2 superblock\n");
			_superBlock = sb;
			initSuperBlock();
		} else {
			_valid = false;
			_nbGroupBlock = 0;
			_superBlock = nullptr;
			_rootInode = nullptr;
			root = nullptr;
		}
	}

	void initSuperBlock() {
		_valid = true;
		_logBlockSize = 10 + _superBlock->log_block_size;

		_nbGroupBlock = roundUp(_superBlock->block_group_blocks, _superBlock->nb_blocks);
		if (_superBlock->version_major < 1)
			_inodeStructSize = 128;
		else
			_inodeStructSize = _superBlock->size_of_inode_struct;

		readBlockGroupDescTable();

		_rootInode = getInode(2);
		root = new Ext2Dentry(_rootInode);
		// TODO set root Dentry*
	}

	void readBlockGroupDescTable() {
		_blockGroupDescTable = new ext2_block_group_desc_t[_nbGroupBlock];
		std::uint64_t blockStart = 1;
		if(getBlockSize()==1024)
			blockStart=2;
		std::uint64_t nbBlock = roundUp(_nbGroupBlock * sizeof(ext2_block_group_desc_t), getBlockSize());
		std::uint64_t offset = 0;
		std::uint64_t maxSize = sizeof(ext2_block_group_desc_t) * _nbGroupBlock;
		for (std::uint64_t i = blockStart; i < blockStart + nbBlock; i++) {
			std::uint64_t sectorNo;
			std::uint32_t sectorCount;
			blockToSector(i, &sectorNo, &sectorCount);
			for (std::uint64_t j = sectorNo; j < sectorNo + sectorCount; j++) {
				Block* b = _dev->getBlock(j);
				void* source = ((char*)b->page->kernelMappAddr) + b->offset;
				void* destination = ((char*)_blockGroupDescTable) + offset;
				std::size_t size = std::min(maxSize - offset, _dev->getSectorSize());
				if (size == 0) {
					break;
				}
				memcpy(destination, source, size);
				offset += size;
			}
		}
	}

	static uint64_t roundUp(std::uint64_t num, std::uint64_t den) {
		 if(den == 0)
			 return 0;
		 if( num % den == 0)
			 return num/den;
		 else
			 return num/den+1;
	}

	virtual Inode* allocInode() override {
		return nullptr;
	}

	Ext2Inode* getInode(std::uint32_t ino) {
		std::uint64_t blockGroupNo = (ino - 1) / _superBlock->block_group_inodes;
		std::uint64_t blockGroupIndex = (ino - 1) % _superBlock->block_group_inodes;
		std::uint64_t blockNo = _blockGroupDescTable[blockGroupNo].start_inode_table + ((blockGroupIndex * _inodeStructSize) / getBlockSize());
		std::uint64_t blockOffset = (blockGroupIndex * _inodeStructSize) % getBlockSize();
		std::uint64_t sectorStart;
		std::uint32_t sectorCount;
		blockToSector(blockNo, &sectorStart, &sectorCount);
		std::uint64_t sectorNo = sectorStart + blockOffset / _dev->getSectorSize();
		Block* b = _dev->getBlock(sectorNo);
		std::uint64_t sectorOffset = blockOffset % _dev->getSectorSize();
		ext2_inode_data_t* inodeData = new ext2_inode_data_t();
		void* source = ((char*)b->page->kernelMappAddr) + b->offset + sectorOffset;
		void* destination = inodeData;
		std::size_t size = _inodeStructSize;
		memcpy(destination, source, size);
		Ext2Inode* inode = new Ext2Inode(this, ino, inodeData);
		return inode;
	}

	void blockToSector(const std::uint32_t blockNo, std::uint64_t* sectorNo, std::uint32_t* sectorCount) {
		// TODO implement _dev->getLockSectorSize
		*sectorCount = 1 << (_logBlockSize - 9);
		*sectorNo = blockNo * *sectorCount;
	}

	/*void readPage(std::uint32_t bno_start, Page* p) {
		std::uint64_t lba_start = bno_start * getBlockSize() / _dev->getSectorSize();
		std::uint64_t lba_end = lba_start + PAGE_SIZE / _dev->getSectorSize();
		for (std::uint64_t i = lba_start; i < lba_end; i++) {
			Block* blk = _dev->getBlock(i);
			void* src = (void*)(((char*)blk->page->kernelMappAddr) + blk->offset);
			void* dest = (void*)(((char*)p->kernelMappAddr) + blk->offset);
			memcpy(dest, src, _dev->getSectorSize());
		}
	}*/

	std::uint64_t getBlockSize() {
		return 1 << _logBlockSize;
	}

	BlockDevice* _dev;
	bool _valid;
	std::uint64_t _nbGroupBlock;
	ext2_superblock_t* _superBlock;
	ext2_block_group_desc_t* _blockGroupDescTable;
	Ext2Inode* _rootInode;
	std::uint32_t _logBlockSize;
	std::uint32_t _inodeStructSize;

};
