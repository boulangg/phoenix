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

enum NumberLines
{
    All,
    NonBlank,
    None
};

int cat(FILE* fd, bool showEnd, bool showTab, bool showNonPrinting, bool squeezeBlank, NumberLines numberLines)
{
    char buffer[BUFFER_SIZE];
    while (fgets(buffer, BUFFER_SIZE - 1, fd) != nullptr) {
        if (showEnd) {
        }

        printf("%s", buffer);
    }
    return 0;
}

int main(int argc, char* argv[])
{
    static cmd::Config config = {
        "cat", cmd::Version{1, 0, 0}, "[OPTION]... [FILE]...",
        "Concatenate FILE(s) to standard output.\n\n"
        "With no FILE, or when FILE is -, read standard input.",
        std::vector<cmd::Option>{
            cmd::Option{'a', "show-all", false, "equivalent to -vET"},
            cmd::Option{'b', "number-nonblank", false, "number nonempty output lines, overrides -n"},
            cmd::Option{'e', {}, false, "equivalent to -vE"},
            cmd::Option{'E', "show-ends", false, "display $ at end of each line"},
            cmd::Option{'n', "number", false, "number all output lines"},
            cmd::Option{'s', "squeeze-blank", false, "suppress repeated empty output lines"},
            cmd::Option{'t', {}, false, "equivalent to -vT"},
            cmd::Option{'T', "show-tabs", false, "display TAB characters as ^I"},
            cmd::Option{'u', {}, false, "(ignored)"},
            cmd::Option{'v', "show-nonprinting", false, "use ^ and M- notation, except for LFD and TAB"}}};

    cmd::Parser parser{config};

    parser.parse(argc, argv);

    bool showEnd = parser.isEnabled("E");
    bool showTab = parser.isEnabled("T");
    bool showNonPrinting = parser.isEnabled("v");

    if (parser.isEnabled("a")) {
        showNonPrinting = true;
        showEnd = true;
        showTab = true;
    }

    if (parser.isEnabled("e")) {
        showNonPrinting = true;
        showEnd = true;
    }

    if (parser.isEnabled("t")) {
        showNonPrinting = true;
        showTab = true;
    }

    bool squeezeBlank = parser.isEnabled("s");
    NumberLines numberLines = NumberLines::None;
    if (parser.isEnabled("n")) {
        numberLines = NumberLines::All;
    }
    if (parser.isEnabled("b")) {
        numberLines = NumberLines::NonBlank;
    }

    FILE* fd = stdin;
    if (argc > 1) {
        std::string filename = argv[1];
        fd = fopen(filename.c_str(), "r");
    }

    cat(fd, showEnd, showTab, showNonPrinting, squeezeBlank, numberLines);

    return 0;
}
