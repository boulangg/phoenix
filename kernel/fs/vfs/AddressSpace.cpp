#include "AddressSpace.hpp"

#include <cstring>

#include <include/constant.h>
#include <mm/PhysicalAllocator.hpp>

#include "Inode.hpp"


AddressSpace::AddressSpace(Inode* host) :
		host(host), pages()
{

}

AddressSpace::~AddressSpace() {
	for (Page* p : pages) {
		PhysicalAllocator::freePage(p);
	}
}

Page* AddressSpace::getPage(size_t pageNo) {
	// TODO Check that the pageNo correspond in the inode?
	while (pageNo >= pages.size()) {
		Page* p = PhysicalAllocator::allocPage();
		p->offset = PAGE_SIZE*pages.size();
		pages.push_back(p);
	}
	return pages[pageNo];
}

// adress_space_operation
//int (*writepage)(struct page *page, struct writeback_control *wbc);
int AddressSpace::readPage(Page *p) {
	// Just zeroed the content of the page
	memset(p->kernelMappAddr, 0, PAGE_SIZE);
	return 0;
}
