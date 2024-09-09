/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #include "cmd_utils.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

/*
    Code below is licensed under CC0 1.0.
*/

#include <cstdio>
#include <stdexcept>
#include <string>

template <typename... Args>
std::string string_format(const std::string& format, Args... args)
{
    int size_s = std::snprintf(nullptr, 0, format.c_str(), args...) + 1; // Extra space for '\0'
    if (size_s <= 0) {
        throw std::runtime_error("Error during formatting.");
    }
    auto size = static_cast<size_t>(size_s);
    char* buf = new char[size];
    std::snprintf(buf, size, format.c_str(), args...);
    auto ret = std::string(buf, size - 1); // We don't want the '\0' inside
    delete buf;
    return ret;
}
/*
    End of code licensed under CC0 1.0.
*/

namespace cmd {

Config::Config(std::string name, Version version, std::string usage, std::string helpDescription,
               std::set<Option> options) :
    name(name),
    version(version), usage(usage), helpDescription(helpDescription), options(options)
{
    static Option helpOption{'\0', "help", true, false, false, "display this help and exit"};
    static Option versionOption('\0', "version", true, false, false, "output version information and exit");
    this->options.insert(helpOption);
    this->options.insert(versionOption);
}

Parser::Parser(const Config& config) : _config(config) {}

void Parser::parse(int argc, char** argv)
{
    _arg0 = argv[0];
    int i = 1;
    // Parse options
    for (; i < argc; ++i) {
        if (std::strcmp(argv[i], "--help") == 0) {
            helpAndExit();
        }
        if (std::strcmp(argv[i], "--version") == 0) {
            versionAndExit();
        }
    }

    // Parse operands
    for (; i < argc; ++i) {
        _operands.push_back(argv[i]);
    }
}

std::vector<std::string> Parser::operands()
{
    return _operands;
}

void Parser::helpAndExit()
{
    printf("Usage: %s %s\n", _arg0.c_str(), _config.usage.c_str());
    printf("%s\n", _config.helpDescription.c_str());

    printf("Options:\n");

    for (auto& option : _config.options) {
        std::string shortOption = "  ";
        if (option.shortName != '\0') {
            shortOption = string_format("-%c", option.shortName);
        }
        std::string delim = "  ";
        if (option.shortName != '\0' && option.longName != "") {
            delim = ", ";
        }
        std::string longOption = "                        ";
        if (option.longName != "") {
            longOption = string_format("--%-23s", option.longName.c_str());
        }
        printf("  %s%s%s ", shortOption.c_str(), delim.c_str(), longOption.c_str());
        if (longOption.size() > 25) {
            printf("\n                               ");
        }
        printf(" %s\n", option.description.c_str());
    }

    std::exit(0);
}

void Parser::versionAndExit()
{
    printf("%s %i.%i.%i\n", _config.name.c_str(), _config.version.major, _config.version.minor, _config.version.patch);
    std::exit(0);
}

}
