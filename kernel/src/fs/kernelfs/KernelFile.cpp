/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "KernelFile.h"

#include <algorithm>
#include <cstring>

namespace kernel::fs::kernelfs {

KernelFile::KernelFile(std::uint64_t startAddr, std::size_t fileSize) :
    File(nullptr), _startAddr(startAddr), _fileSize(fileSize)
{}

ssize_t KernelFile::read(char* buffer, size_t size)
{
    size_t nr = std::min<std::size_t>(size, _fileSize - _pos);
    std::memcpy(buffer, (char*)(_startAddr + _pos), nr);
    return nr;
}

ssize_t KernelFile::write(char* buffer, size_t size)
{
    (void)buffer;
    (void)size;
    return 0;
}

loff_t KernelFile::llseek(loff_t offset, std::int32_t origin)
{
    loff_t retval;
    switch (origin) {
    case SEEK_END:
        offset += _fileSize;
        break;
    case SEEK_CUR:
        offset += _pos;
    }
    retval = -EINVAL;
    if (offset >= 0 && static_cast<std::size_t>(offset) <= _fileSize) {
        if (offset != _pos) {
            _pos = offset;
        }
        retval = offset;
    }

    return retval;
}
}
