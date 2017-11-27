#pragma once

#include <cstdint>

#include <cstdio>
#include <cstdlib>

#include <core/Console.hpp>

#include "BlockCache.hpp"
#include "BlockStorageDevice.hpp"

struct PartitionInfo {
	std::uint64_t startSector;
	std::uint64_t nbSectors;
	int partno;
};

struct MBR {
	char bootstrap[436];
	char diskID[10];
	char MBRPartitionTable[64];
	char signature[2];
};

static_assert(sizeof(MBR) == 512, "Size is not correct");

struct MBRPartition {
	union {
		struct {
			uint8_t partEntry[16];
		};
		struct {
			std::uint8_t bootIndicator;
			std::uint8_t startHead;
			std::uint8_t startSector;
			std::uint8_t startCylinder;
			std::uint8_t SystemID;
			std::uint8_t endHead;
			std::uint8_t endSector;
			std::uint8_t endCylinder;
			std::uint32_t partitionStartLBA;
			std::uint32_t partitionSize;
		};
	};
};


class Disk {
public:
	Disk(BlockStorageDevice* storage) : storage(storage), parts(), cache(storage->getSectorSize()) {
		readMBR();
	}

	virtual ~Disk() {

	}

	PartitionInfo getPartitionInfo(int partno)  {
		if (partno < 0 || partno >= (int)parts.size()) {
			return PartitionInfo();
		} else {
			return parts[partno];
		}
	}

	Block* getBlock(std::uint64_t lba) {
		BlockIO bio;
		BlockIOSegment bioSeg1;
		Block* block = cache.getBlock(lba);
		if (block->valid) {
			return block;
		} else {
			bioSeg1.page = block->page;
			bioSeg1.offset = block->offset;
			bioSeg1.len = block->len;
			bio.segments.push_back(bioSeg1);
			bio.nb_sectors = 1;
			bio.start_sector = lba;
			bio.write = 0;
			storage->loadBlockIO(bio);

			block->valid = true;
			return block;
		}
	}

	std::uint64_t getSectorSize() {
		return storage->getSectorSize();
	}

private:

	bool readMBR() {
		char tmp[1024];
		sprintf(tmp, "Informations about '%s'\n", storage->getName().c_str());
		Console::write(tmp);

		Block* block = getBlock(0);
		MBR* mbr = (MBR*)(block->page->kernelMappAddr + block->offset);
		if ((mbr->signature[0] == (char)0x55) && (mbr->signature[1] == (char)0xAA)) {
			PartitionInfo partFull = {0, storage->getSectorNumber(), 0};
			parts.push_back(partFull);
			for (int partNum = 0; partNum < 4; partNum++) {
				MBRPartition part;
				for (int i = 0; i < 16; i++) {
					part.partEntry[i] = mbr->MBRPartitionTable[partNum*16+i];
				}
				sprintf(tmp, "Partition %i size: %i sectors, start: %i\n", partNum,
						part.partitionSize, part.partitionStartLBA);
				Console::write(tmp);

				PartitionInfo newPart = {part.partitionStartLBA, part.partitionSize, partNum};
				parts.push_back(newPart);

			}
			return true;
		} else {
			return false;
		}

	}

	BlockStorageDevice* storage;
	std::vector<PartitionInfo> parts;
	BlockCache cache;
};
