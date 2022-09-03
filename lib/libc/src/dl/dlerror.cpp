#include "dl_internal.h"

#include <stddef.h>
#include <string.h>

#define DL_ERROR_BUFFER_SIZE 127
char* _dlError = NULL;
char _dlErrorBuffer[DL_ERROR_BUFFER_SIZE + 1] = "";

char* dlerror(void)
{
	if (_dlError == NULL) {
		return NULL;
	} else {
		char* tmp = _dlError;
		_dlError = NULL;
		return tmp;
	}
}

void setDlError(const char* errorMessage)
{
	size_t errorMsgSize = strlen(errorMessage);
	strncpy(_dlErrorBuffer, errorMessage, DL_ERROR_BUFFER_SIZE);
	_dlErrorBuffer[errorMsgSize] = '\0';
	_dlError = _dlErrorBuffer;
}
