#pragma once

#include <cstdint>
#include <string>

#include "BlockDevice.hpp"

class Partition : public BlockDevice {
public:
	Partition(Disk* storage, int partNo, std::string name);
	virtual ~Partition();


	virtual std::uint64_t getStorageSize() {
		return getNbSectors() * getSectorSize();
	}

	virtual std::uint64_t getNbSectors() {
		return storage->getPartitionInfo(partNo).nbSectors;
	}

	virtual std::uint64_t getSectorSize() {
		return storage->getSectorSize();
	}

	virtual std::uint64_t getPartitionStart() {
		return storage->getPartitionInfo(partNo).startSector;
	}

	virtual Block* getBlock(std::uint64_t lba) {
		return storage->getBlock(lba + getPartitionStart());
	}

	virtual const std::string& getName() const {
		return name;
	}

private:
	Disk* storage;
	int partNo;
	std::string name;
};
