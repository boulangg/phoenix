/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <algorithm>

#include <fs/Inode.h>

#include "RamSuperBlock.h"

namespace kernel::fs::ramfs {

class RamInode : public Inode
{
public:
    RamInode(RamSuperBlock* sb);
    ~RamInode();

    Inode* lookup(const std::string& pathNameComponent) override;

    void readpage(mem::Page* p, std::size_t pageNum) override;

    void mkdir(std::string dirName) override;

private:
    std::unordered_map<std::string, Inode*> _children;
    RamSuperBlock* _sb;
};

}
