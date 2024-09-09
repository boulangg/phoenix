/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include <stdlib.h>
#include <string.h>

#include <cmd_utils.h>
#include <cstdio>

#define BUFFER_SIZE 32

int main(int argc, char* argv[])
{
    static cmd::Config config = {
        "cat", cmd::Version{1, 0, 0}, "[OPTION]... [FILE]...",
        "Concatenate FILE(s) to standard output.\n\nWith no FILE, or when FILE is -, read standard input.",
        std::set<cmd::Option>{
            cmd::Option{'a', "show-all", true, false, false, " quivalent to -vET"},
            cmd::Option{'b', "number-nonblank", true, false, false, "number nonempty output lines, overrides -n"},
            cmd::Option{'e', "", true, false, false, "equivalent to -vE"},
            cmd::Option{'E', "show-ends", true, false, false, "display $ at end of each line"},
            cmd::Option{'n', "number", true, false, false, "number all output lines"},
            cmd::Option{'s', "squeeze-blank", true, false, false, "suppress repeated empty output lines"},
            cmd::Option{'t', "", true, false, false, "equivalent to -vT"},
            cmd::Option{'T', "show-tabs", true, false, false, "display TAB characters as ^I"},
            cmd::Option{'u', "", true, false, false, "(ignored)"},
            cmd::Option{'v', "show-nonprinting", true, false, false,
                        "use ^ and M- notation, except for LFD and TAB"}}};

    FILE* fd;
    if (argc > 1) {
        std::string filename = argv[1];
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
