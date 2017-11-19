#pragma once

#include <cstdint>
#include <vector>

#include <mm/Page.hpp>

typedef std::uint64_t sector_t;

struct BlockIOSegment {
	Page* page;
	std::uint64_t offset;
	std::uint64_t len;
};

enum BlockIOStatus {
	FAILED,
	PENDING,
	INPROGRESS,
	COMPLETED
};

struct BlockIO {

	sector_t start_sector;
	size_t nb_sectors;
	std::vector<BlockIOSegment> segments;
	bool write;

	void setStatus(BlockIOStatus) {}
};
