/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <errno.h>

#include <sys/types.h>

namespace kernel::fs {

class File
{
public:
    File();
    virtual ~File();

    // File Operations
    virtual ssize_t read(char* buffer, std::size_t size) = 0;
    virtual ssize_t write(char* buffer, std::size_t size) = 0;
    virtual loff_t llseek(loff_t offset, std::int32_t origin) = 0;

protected:
    loff_t _pos;
};

}
