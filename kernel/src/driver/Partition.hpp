#pragma once

#include <cstdint>
#include <string>

#include "BlockDevice.hpp"

class Disk;

class Partition : public BlockDevice
{
public:
	Partition(Disk* storage, int partNo, std::string name);
	virtual ~Partition();


	virtual std::uint64_t getStorageSize() override;

	virtual std::uint64_t getSectorNumber() override;

	virtual std::uint64_t getSectorSize() override;

	virtual std::uint64_t getPartitionStart();

	virtual Block* getBlock(std::uint64_t lba) override;

	virtual const std::string& getName() const override;

private:
	Disk* storage;
	int partNo;
	std::string name;
};
