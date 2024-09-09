/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <cstdint>
#include <set>
#include <string>
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
    char shortName = '\0';
    std::string longName = "";
    bool isOptional = false;
    bool hasArgument = false;
    bool multiOccurences = false;
    std::string description = "";

    friend std::strong_ordering operator<=>(const Option& l, const Option& r)
    {
        return l.getBaseName() <=> r.getBaseName();
    }

private:
    std::string getBaseName() const
    {
        if (shortName != '\0') {
            return std::string(1, shortName);
        } else {
            return longName;
        }
    }
};

struct Config
{
    std::string name;
    Version version;
    std::string usage;
    std::string helpDescription;
    std::set<Option> options;

    Config(std::string name, Version version, std::string usage, std::string helpDescription,
           std::set<Option> options);
};

class UtilityArgumentsConfig
{
public:
    UtilityArgumentsConfig(const std::string& programName, Version version, const std::string& description);

    void addOption(const Option& option);
};

class Parser
{
public:
    Parser(const Config& config);

    void parse(int argc, char** argv);

    bool isEnabled(std::string optionName);
    std::size_t count(std::string optionName);
    std::vector<std::string> values(std::string optionName);

    std::vector<std::string> operands();

private:
    void helpAndExit();
    void versionAndExit();

    Config _config;
    std::string _arg0;
    std::vector<std::string> _operands;
};

}
