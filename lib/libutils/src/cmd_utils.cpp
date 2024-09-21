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

Parser::Parser(const Config& config) : _config(config), _optionArgId(), _arg0(), _arguments(), _operands()
{
    for (std::size_t i = 0; i < _config.options.size(); ++i) {
        auto& option = _config.options[i];
        if (option.shortName.has_value()) {
            _optionArgId[std::string(1, option.shortName.value())] = i;
        }
        if (option.longName.has_value()) {
            _optionArgId[option.longName.value()] = i;
        }
    }
}

void Parser::parse(int argc, char** argv)
{
    _arg0 = argv[0];
    int i = 1;
    // Parse options
    for (; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--help") {
            helpAndExit();
        }
        if (arg == "--version") {
            versionAndExit();
        }

        if (arg[0] == '-') {
            if (arg[1] == '-') {
                // longName
                auto longName = arg.substr(2);
                if (!_optionArgId.contains(longName)) {
                    unrecognizedOptionAndExit(longName);
                }
                if (!_arguments.contains(_optionArgId.at(longName))) {
                    _arguments[_optionArgId.at(longName)] = {};
                }
                auto& option = _config.options[_optionArgId.at(longName)];
                if (option.hasArgument && argv[i + 1][0] != '\0' && argv[i + 1][0] != '-') {
                    _arguments[_optionArgId.at(longName)].push_back(argv[i + 1]);
                    ++i;
                }
            } else {
                // shortName
                for (auto j = 2; arg[j] != '\0'; ++j) {
                    auto shortName = std::string(1, arg[j]);
                    if (!_optionArgId.contains(shortName)) {
                        unrecognizedOptionAndExit(shortName);
                    }
                    if (!_arguments.contains(_optionArgId.at(shortName))) {
                        _arguments[_optionArgId.at(shortName)] = {};
                    }
                    auto& option = _config.options[_optionArgId.at(shortName)];
                    if (arg[j + 1] != '\0') {
                        _arguments[_optionArgId.at(shortName)].push_back(arg.substr(j+1));
                        break;
                    }
                    if (option.hasArgument && argv[i + 1][0] != '\0' && argv[i + 1][0] != '-') {
                        _arguments[_optionArgId.at(shortName)].push_back(argv[i + 1]);
                        ++i;
                    }
                }
            }
        } else {
        // operand
            _operands.push_back(argv[i]);
        }
    }
}

bool Parser::isEnabled(std::string optionName) const
{
    if (!_optionArgId.contains(optionName)) {
        throw std::invalid_argument("Unknown option name: " + optionName);
    }
    return _arguments.contains(_optionArgId.at(optionName));
}

std::size_t Parser::count(std::string optionName) const
{
    if (!_optionArgId.contains(optionName)) {
        throw std::invalid_argument("Unknown option name: " + optionName);
    }
    return _arguments.at(_optionArgId.at(optionName)).size();
}

std::vector<std::string> Parser::values(std::string optionName) const
{
    if (!_optionArgId.contains(optionName)) {
        throw std::invalid_argument("Unknown option name: " + optionName);
    }
    return _arguments.at(_optionArgId.at(optionName));
}

void Parser::enable(std::string optionName)
{
    if (!_optionArgId.contains(optionName)) {
        throw std::invalid_argument("Unknown option name: " + optionName);
    }

    if (!_arguments.contains(_optionArgId[optionName])) {
        _arguments[_optionArgId[optionName]] = {};
    }
}
void Parser::disable(std::string optionName)
{
    if (!_optionArgId.contains(optionName)) {
        throw std::invalid_argument("Unknown option name: " + optionName);
    }

    _arguments.erase(_optionArgId[optionName]);
}

std::vector<std::string> Parser::operands()
{
    return _operands;
}

const Option Parser::HELP_OPTION{'\0', "help", false, "display this help and exit"};
const Option Parser::VERSION_OPTION('\0', "version", false, "output version information and exit");

void Parser::displayOption(Option option)
{
    std::string shortOption = "  ";
    if (option.shortName.has_value()) {
        shortOption = string_format("-%c", option.shortName.value());
    }
    std::string delim = "  ";
    if (option.shortName.has_value() && option.longName.has_value()) {
        delim = ", ";
    }
    std::string longOption = "                        ";
    if (option.longName.has_value()) {
        longOption = string_format("--%-23s", option.longName.value().c_str());
    }
    printf("  %s%s%s ", shortOption.c_str(), delim.c_str(), longOption.c_str());
    if (longOption.size() > 25) {
        printf("\n                               ");
    }
    printf(" %s\n", option.description.c_str());
}

void Parser::helpAndExit()
{
    printf("Usage: %s %s\n", _arg0.c_str(), _config.usage.c_str());
    printf("%s\n", _config.helpDescription.c_str());

    printf("Options:\n");

    for (auto& option : _config.options) {
        displayOption(option);
    }

    displayOption(HELP_OPTION);
    displayOption(VERSION_OPTION);

    std::exit(0);
}

void Parser::versionAndExit()
{
    printf("%s %i.%i.%i\n", _config.name.c_str(), _config.version.major, _config.version.minor, _config.version.patch);
    std::exit(0);
}

void Parser::unrecognizedOptionAndExit(std::string optionName)
{
    printf("%s: unrecognized option -- '%s' \n", _arg0.c_str(), optionName.c_str());
    printf("Try '%s --help' for more information.\n", _arg0.c_str());
    std::exit(1);
}
}
