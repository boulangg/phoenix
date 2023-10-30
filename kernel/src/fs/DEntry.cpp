/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "DEntry.h"

namespace kernel::fs {

DEntry::DEntry(Inode* inode) : _parent(this), _inode(inode), _name("") {}
DEntry::DEntry(DEntry* parent, Inode* inode, std::string name) : _parent(parent), _inode(inode), _name(name) {}

DEntry::~DEntry() {}

}
