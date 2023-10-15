/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

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
