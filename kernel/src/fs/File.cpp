/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #include "File.h"

namespace kernel::fs {

File::File(){};

File::~File(){};

// File Operations
ssize_t File::read(char* buffer, std::size_t size)
{
    ssize_t bytes = read_internal(buffer, size, _pos);
    _pos += bytes;
    return bytes;
}

ssize_t File::write(char* buffer, std::size_t size)
{
    ssize_t bytes = write_internal(buffer, size, _pos);
    _pos += bytes;
    return bytes;
}

loff_t File::lseek(loff_t offset, std::int32_t origin)
{
    _pos = lseek_internal(offset, origin);
    return _pos;
}

}
