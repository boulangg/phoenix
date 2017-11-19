#pragma once

#include "BlockIO.hpp"

class BlockStorageDevice {
public:
	virtual ~BlockStorageDevice() {}
	// TODO replace by processRequests
	void loadBlockIO(BlockIO bio) {
		processBlockIO(bio);
	}
	//void processRequests(RequestQueue);

	virtual std::uint64_t getStorageSize() = 0;
	virtual std::uint32_t getSectorSize() = 0;

private:
	virtual void processBlockIO(BlockIO) = 0;
};
