/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <cstddef>
#include <unordered_map>

#include <sys/types.h>

#include "KernelGlobals.h"

namespace kernel::fs {

class Inode;

class FileCache
{
public:
    FileCache(Inode* inode);
    ~FileCache();

    ssize_t read(char* buffer, loff_t offset, std::size_t size);

private:
    Inode* _inode;
    std::unordered_map<std::size_t, mem::Page*> _cache;
};

}
