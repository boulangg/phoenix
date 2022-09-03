#pragma once

#include <cstdint>

#include "Elf64File.h"

namespace dl {

#define LOADER_MAX_DL_NUM 256


class Loader
{
public:
	typedef Elf64File* ElfFileId;

	static ElfFileId open(const std::string& filename, int mode);

	static char* error();

	static void* sym(ElfFileId handle, const std::string& symbol);

	static int close(ElfFileId handle);

private:
	static Elf64File* _files[LOADER_MAX_DL_NUM];

#define DL_ERROR_BUFFER_SIZE 127
	static char* _dlError;
	static char _dlErrorBuffer[DL_ERROR_BUFFER_SIZE + 1];

	static void setDlError(const std::string& errorMessage);

	static int getHandlerId(const std::string& filename);
};

}
