/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

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
