/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include <stdio.h>
#include <string>

#define BUFFER_SIZE 32

int main(int argc, char* argv[])
{
    FILE* fd;
    if (argc > 1) {
        std::string filename = argv[1];
        if (filename.empty()) {
            filename = "Makefile";
        }
        fd = fopen(filename.c_str(), "r");
    } else {
        fd = stdin;
    }
    char buffer[BUFFER_SIZE];
    while (fgets(buffer, BUFFER_SIZE - 1, fd) != nullptr) {
        printf("%s", buffer);
    }

    return 0;
}
