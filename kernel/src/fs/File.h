/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <errno.h>

#include <sys/types.h>

#include "Inode.h"

namespace kernel::fs {

class File
{
public:
    File(Inode* inode);
    virtual ~File();

    // File Operations
    virtual ssize_t read(char* buffer, std::size_t size)
    {
        auto count = _inode->read(buffer, size, _pos);
        _pos += count;
        return count;
    }

    virtual ssize_t write(char* buffer, std::size_t size)
    {
        (void)buffer;
        (void)size;
        return 0;
    }

    virtual loff_t llseek(loff_t offset, std::int32_t origin)
    {
        switch (origin) {
        case SEEK_SET:
            _pos = std::max(0ll, offset);
            break;
        case SEEK_CUR:
            _pos += offset;
            _pos = std::max(0ll, _pos);
            break;
        case SEEK_END:
            _pos = _inode->getFileSize();
            _pos = std::max(0ll, _pos);
            break;
        }

        return _pos;
    }

protected:
    loff_t _pos;
    Inode* _inode;
};

}
