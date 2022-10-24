#include <stdio.h>

#include <dlfcn.h>

#include <string>

int main()
{
	/*void* dlHandle = dlopen("/usr/bin/cat", RTLD_LAZY);
	std::string symName = "fprintf";
	void* symAddr = dlsym(dlHandle, symName.c_str());

	printf("%s, %p", symName.c_str(), symAddr);*/
	return 0;
}
