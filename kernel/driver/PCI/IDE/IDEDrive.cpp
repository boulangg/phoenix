#include "IDEDrive.hpp"

#include "IDEChannel.hpp"

IDEDrive::IDEDrive(IDEChannel* channel, bool slave, IDEDiskType type) :
		channel(channel), slave(slave), type(type),
		identifyData(), nbSectors(0), sectorSize(DEFAULT_SECTOR_SIZE)
{

}

IDEDrive::~IDEDrive() {}

void IDEDrive::processBlockIO(BlockIO bio) {
	channel->processBlockIO(bio, slave);
}

std::uint64_t IDEDrive::getStorageSize() {
	return nbSectors*sectorSize;
}


std::uint32_t IDEDrive::getSectorSize() {
	return sectorSize;
}
