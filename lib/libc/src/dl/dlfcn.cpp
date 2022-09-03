#include "dlfcn.h"

#include <string>

#include "Loader.h"

using namespace dl;

void* dlopen(const char* filename, int flag)
{
	return static_cast<Loader::ElfFileId>(Loader::open(std::string(filename), flag));
}

void* dlsym(void* handle, const char* symbol)
{
	return Loader::sym((Loader::ElfFileId)handle, std::string(symbol));
}

int dlclose(void* handle)
{
	return Loader::close((Loader::ElfFileId)handle);
}

char* dlerror(void)
{
	return Loader::error();
}
