#pragma once

#include "BlockIO.hpp"
#include <string>

class BlockStorageDevice {
public:
	virtual ~BlockStorageDevice() {}
	// TODO replace by processRequests
	void loadBlockIO(BlockIO bio) {
		processBlockIO(bio);
	}
	//void processRequests(RequestQueue);

	virtual std::uint64_t getStorageSize() = 0;
	virtual std::uint64_t getSectorNumber() = 0;
	virtual std::uint32_t getSectorSize() = 0;

	const std::string& getName() {
		return name;
	}

	void setName(std::string name) {
		this->name = name;
	}

protected:

private:
	virtual void processBlockIO(BlockIO) = 0;

	std::string name;
};
