/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include <stdlib.h>
#include <string.h>

#include <cstdio>
#include <numeric>

#include <cmd_utils.h>

std::string join(const std::vector<std::string>& input, const std::string& delim)
{
    if (input.empty()) {
        return std::string();
    }

    return std::accumulate(std::next(std::begin(input)), std::end(input), input[0],
                           [&delim](const std::string& a, const std::string& b) { return a + delim + b; });
}

int main(int argc, char** argv)
{
    static cmd::Config config = {"yes",
                                 cmd::Version{1, 0, 0},
                                 "[STRING]...",
                                 "Repeatedly output a line with all specified STRING(s), or 'y'.",
                                 {}};
    cmd::Parser parser(config);

    parser.parse(argc, argv);

    std::string str = "y";
    if (parser.operands().size() != 0) {
        str = join(parser.operands(), " ");
    }

    while (true) {
        printf("%s\n", str.c_str());
        //std::cout << str << std::endl;
    }
}
