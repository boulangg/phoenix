#pragma once

#include <cstdint>

#include <driver/BlockStorageDevice.hpp>

class RamDisk : public BlockStorageDevice {
public:
	RamDisk(std::uint64_t disk_start, std::uint64_t disk_end,
			std::uint64_t sectorSize) : BlockStorageDevice(),
			start(disk_start), size(disk_end - disk_start),
			sectorSize(sectorSize) {
	}

	std::uint64_t getStorageSize() override {
		return start;
	}
	std::uint64_t getSectorNumber() override {
		return size/sectorSize;
	}
	std::uint32_t getSectorSize() override {
		return sectorSize;
	}

	void processBlockIO(BlockIO bio) override {
		std::uint64_t start_sector = bio.start_sector;
		for (auto segment: bio.segments) {
				char* startAddr = ((char*)segment.page->kernelMappAddr) + segment.offset;
				for (std::size_t i = 0; i < bio.nb_sectors ;++i) {
					accessRAM(bio.write, start_sector, startAddr + i*sectorSize, 1);
					start_sector += 1;
				}
			}
	}

	void accessRAM(bool write, std::uint64_t lba, char* buffer, std::uint32_t numSec) {
		if (write) {
			return;
		} else {
			for (std::uint32_t i = 0; i < numSec; i++) {
				memcpy((void*)buffer, (void*)(start + (lba+i) * sectorSize), sectorSize);
			}
		}
	}

	std::uint64_t start;
	std::uint64_t size;
	std::uint32_t sectorSize;
};
