#include "Loader.h"

#include <algorithm>
#include <cstring>

#define LOADER_START_ADDR 	0x08000000ul // 1Go
#define LOADER_SIZE			0x08000000ul // 1Go

using namespace dl;

Loader::ElfFileId Loader::open(const std::string& filename, int mode)
{
	int handlerId = getHandlerId(filename);
	if (handlerId == -1) {
		setDlError("Too much shared file open");
		return nullptr;
	} else if (_files[handlerId] != nullptr) {
		_files[handlerId]->incrementCount();
	} else {
		_files[handlerId] = new Elf64File(filename);
		_files[handlerId]->load((LOADER_START_ADDR + handlerId * LOADER_SIZE), mode);
	}
	return _files[handlerId];
}

char* Loader::error()
{
	auto tmp = _dlError;
	_dlError = nullptr;
	return tmp;
}

void* Loader::sym(ElfFileId handle, const std::string& symbol)
{
	return (void*)handle->getSymbol(symbol);
}

int Loader::close(ElfFileId handle)
{
	handle->decrementCount();
	if (handle->getCount() == 0) {
		for (int i = 0; i < LOADER_MAX_DL_NUM; i++) {
			if (_files[i] == handle) {
				handle->unload();
				_files[i] = nullptr;
				delete handle;
				return 0;
			}
		}
	}
	return 0;
}

Elf64File* Loader::_files[LOADER_MAX_DL_NUM];

char* Loader::_dlError;
char Loader::_dlErrorBuffer[DL_ERROR_BUFFER_SIZE + 1];

void Loader::setDlError(const std::string& errorMessage)
{
	size_t errorMsgSize = errorMessage.size();
	strncpy(_dlErrorBuffer, errorMessage.c_str(), DL_ERROR_BUFFER_SIZE);
	_dlErrorBuffer[std::min<size_t>(errorMsgSize, DL_ERROR_BUFFER_SIZE)] = '\0';
	_dlError = _dlErrorBuffer;
}

int Loader::getHandlerId(const std::string& filename)
{
	int availableHandler = -1;
	for (int i = 0; i < LOADER_MAX_DL_NUM; i++) {
		if (_files[i] != nullptr) {
			if (_files[i]->getFileName() == filename) {
				return i;
			}
		} else if (availableHandler == -1) {
			availableHandler = i;
		}
	}

	return availableHandler;
}
