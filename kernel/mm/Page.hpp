#ifndef __PAGE_HPP__
#define __PAGE_HPP__

enum PageType {
	UNUSABLE,
	FREE,
	ALLOCATED,
	KERNEL,
	CACHE
};

class Page {
public:
	uint64_t* physAddr;
	uint64_t* kernelMappAddr;
	PageType type;
};

#endif // __PAGE_HPP__
