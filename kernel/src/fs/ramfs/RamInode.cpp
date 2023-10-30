/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #include "RamInode.h"

namespace kernel::fs::ramfs {

RamInode::RamInode(RamSuperBlock* sb) : Inode(0), _sb(sb) {}

RamInode::~RamInode() {}

Inode* RamInode::lookup(const std::string& pathNameComponent)
{
    if (_children.contains(pathNameComponent)) {
        return _children[pathNameComponent];
    }
    return nullptr;
}

void RamInode::readpage(mem::Page* p, std::size_t pageNum)
{
    (void)pageNum;
    std::fill_n((char*)(p->getKernelAddr()), PAGE_SIZE, 0);
}

void RamInode::mkdir(std::string dirName)
{
    // TODO Check if folder already exist
    _children[dirName] = new RamInode(_sb);
}

}
