#include "stdlib.h"

#include <stddef.h>
#include <string.h>

extern char** environ;

const char* getenv(const char* varName)
{
	size_t i = 0;
	do {
		int varNameLength = strlen(varName);
		if (strncmp(varName, environ[i], varNameLength) == 0 && *(environ[i] + varNameLength) == '=') {
			return environ[i] + varNameLength + 1;
		}
	} while (environ[++i] != 0);

	return NULL;
}
