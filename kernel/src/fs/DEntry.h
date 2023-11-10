/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

namespace kernel::fs {

class DEntry
{
public:
    DEntry(Inode* inode);
    DEntry(DEntry* parent, Inode* inode, std::string name);
    virtual ~Dentry();

private:
    DEntry* _parent;
    Inode* _inode;
    std::string _name;
};

}
