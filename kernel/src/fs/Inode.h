/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include <list>
#include <string>

#include "FileCache.h"

namespace kernel::fs {

class DEntry;

class Inode
{
public:
    Inode(std::uint64_t fileSize);
    virtual ~Inode();

    virtual Inode* lookup(const std::string& pathNameComponent) = 0;
    virtual void readpage(mem::Page* p, std::size_t pageNum) = 0;
    virtual void mkdir(std::string dirName) = 0;

    std::uint64_t getFileSize()
    {
        return _fileSize;
    }

    ssize_t read(char* buffer, loff_t offset, std::size_t size)
    {
        return _fileCache->read(buffer, offset, size);
    }

private:
    std::uint64_t _fileSize;
    FileCache* _fileCache;
    std::list<DEntry*> _dentries;
};

}
