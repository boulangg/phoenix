#include "Ext2AddressSpace.hpp"

int Ext2AddressSpace::readPage_internal(Page* p) {
	Ext2SuperBlock* sb = _host->sb;
	std::uint64_t nbBlock = PAGE_SIZE / sb->getBlockSize();
	std::uint64_t offset = 0;
	for (std::uint32_t i = _host->getBlockNum(p->offset); i < _host->getBlockNum(p->offset) + nbBlock; i++) {
		std::uint64_t sectorNo;
		std::uint32_t sectorCount;
		sb->blockToSector(i, &sectorNo, &sectorCount);
		for (std::uint64_t j = sectorNo; j < sectorNo + sectorCount; j++) {
			Block* b = sb->_dev->getBlock(j);
			void* source = ((char*)b->page->kernelMappAddr) + b->offset;
			void* destination = ((char*)p->kernelMappAddr) + offset;
			std::size_t size =sb->_dev->getSectorSize();
			if (size == 0) {
				break;
			}
			memcpy(destination, source, size);
			offset += size;
		}
	}
	return offset;
}
