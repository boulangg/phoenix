#pragma once

#include <cstdint>
#include <string>

#include "BlockCache.hpp"

class BlockDevice
{
public:
	virtual ~BlockDevice()
	{

	}

	virtual std::uint64_t getStorageSize() = 0;
	virtual std::uint64_t getSectorNumber() = 0;
	virtual std::uint64_t getSectorSize() = 0;

	virtual Block* getBlock(std::uint64_t lba) = 0;

	virtual const std::string& getName() const = 0;
};
