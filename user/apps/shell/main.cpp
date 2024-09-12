/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #include <stdio.h>
#include <string>

int main(int argc, char** argv, char** envp)
{
    for (int i = 0; i < argc; ++i) {
        printf("arg %i: %s\n", i, argv[i]);
    }

    for (int j = 0; envp[j] != NULL; ++j) {
        printf("env %i: %s\n", j, envp[j]);
    }

    return 0;
}
