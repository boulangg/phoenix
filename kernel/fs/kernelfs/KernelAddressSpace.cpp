#include "KernelAddressSpace.hpp"

int KernelAddressSpace::doReadPage(Page* p) {
	char* destination = (char*)p->kernelMappAddr;
	char* source = ((char*)_host->app.apps_start) + p->offset*PAGE_SIZE;
	uint64_t size = std::min((uint64_t)PAGE_SIZE, (uint64_t)(_host->app.apps_end - _host->app.apps_start));
	memcpy(destination, source, size);
	return 0;
}
