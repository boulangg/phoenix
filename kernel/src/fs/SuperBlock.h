/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include <cstdint>

#include "Inode.h"

namespace kernel::fs {

class SuperBlock
{
public:
    SuperBlock() {}
    virtual ~SuperBlock() {}

    Inode* getRootInode()
    {
        return _rootInode;
    }

protected:
    std::uint64_t _blockSize;
    Inode* _rootInode;
};

}
