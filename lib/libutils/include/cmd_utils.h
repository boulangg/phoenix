/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace cmd {

struct Version
{
    std::uint8_t major;
    std::uint8_t minor;
    std::uint8_t patch;
};

struct Option
{
    std::optional<char> shortName = std::nullopt;
    std::optional<std::string> longName = std::nullopt;
    bool hasArgument = false;
    std::string description = "";
};

struct Config
{
    std::string name;
    Version version;
    std::string usage;
    std::string helpDescription;
    std::vector<Option> options;
};

class Parser
{
public:
    Parser(const Config& config);

    void parse(int argc, char** argv);

    bool isEnabled(std::string optionName) const;
    std::size_t count(std::string optionName) const;
    std::vector<std::string> values(std::string optionName) const;

    void enable(std::string optionName);
    void disable(std::string optionName);

    std::vector<std::string> operands();

private:
    static const Option HELP_OPTION;
    static const Option VERSION_OPTION;

    void displayOption(Option option);
    void helpAndExit();
    void versionAndExit();
    void unrecognizedOptionAndExit(std::string optionName);

    Config _config;
    std::unordered_map<std::string, std::uint64_t> _optionArgId;

    std::string _arg0;
    std::unordered_map<std::uint64_t, std::vector<std::string>> _arguments;
    std::vector<std::string> _operands;
};

}
