#pragma once

#include <cstdint>
#include <list>
#include <vector>
#include <include/constant.h>

#include <mm/PhysicalAllocator.hpp>

struct Block {
	Page* page;
	std::uint64_t len;
	std::uint64_t offset;
	bool valid;
	std::uint64_t lba;
	bool dirty;
};


class BlockCache {

	struct BlockCacheData {
		Page* page;
		std::uint64_t lbaStart;
		std::uint64_t lbaEnd;
		std::vector<Block*> blocks;
	};

public:

	BlockCache(std::uint64_t blockSize) : blockData(), blockSize(blockSize) {

	}

	Block* getBlock(std::uint64_t lba) {
		for (auto data: blockData) {
			if (data->lbaStart >= lba && data->lbaEnd < lba) {
				return data->blocks[lba - data->lbaStart];
			} else {
				continue;
			}
		}
		BlockCacheData* data = nullptr;
		data = new BlockCacheData();
		Page* newPage = PhysicalAllocator::allocZeroedPage();
		data->page = newPage;
		std::uint64_t nbBlockPerPage = PAGE_SIZE/blockSize;
		data->lbaStart = (lba / nbBlockPerPage)*nbBlockPerPage;
		data->lbaEnd = data->lbaStart + nbBlockPerPage;
		for (std::uint64_t i = 0; i < nbBlockPerPage; i++) {
			Block* newBlock = new Block();
			newBlock->lba = data->lbaStart+i;
			newBlock->page = newPage;
			newBlock->len = blockSize;
			newBlock->offset = blockSize*i;
			newBlock->dirty = false;
			newBlock->valid = false;
			data->blocks.push_back(newBlock);
		}
		blockData.push_back(data);
		return (data->blocks[lba - data->lbaStart]);
	}

	void releaseBlock(Block* block) {
		(void)block;
	}


private:
	std::list<BlockCacheData*> blockData;
	std::uint64_t blockSize;

};
