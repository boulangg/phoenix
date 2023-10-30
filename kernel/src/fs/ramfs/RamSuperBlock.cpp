/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #include "RamSuperBlock.h"

#include "RamInode.h"

namespace kernel::fs::ramfs {

RamSuperBlock::RamSuperBlock() : SuperBlock()
{
    _rootInode = new RamInode(this);
}

RamSuperBlock::~RamSuperBlock() {}

}
