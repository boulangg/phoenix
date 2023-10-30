/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include "fs/Inode.h"

#include "Ext2Struct.h"

namespace kernel::fs::ext2fs {

class Ext2SuperBlock;

class Ext2Inode : public Inode
{
public:
    Ext2Inode(Ext2SuperBlock* sb, ext2_inode_data inodeData);
    ~Ext2Inode();

    Inode* lookup(const std::string& pathNameComponent) override;
    void readpage(mem::Page* p, std::size_t pageNum) override;

    void mkdir(std::string dirName) override
    {
        (void)dirName;
    }

private:
    Ext2SuperBlock* _sb;
    ext2_inode_data _inodeData;
};

}
