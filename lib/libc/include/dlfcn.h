#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define RTLD_LAZY	0x1
#define RTLD_NOW	0x2
#define RTLD_LOCAL	0x4
#define RTLD_GLOBAL	0x8

	void* dlopen(const char* filename, int flag);
	void* dlsym(void* handle, const char* symbol);
	int dlclose(void* handle);
	char* dlerror(void);

#ifdef __cplusplus
}
#endif
