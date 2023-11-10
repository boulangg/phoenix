/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <list>

#include "fs/DEntry.h"

namespace kernel::fs {

class Inode
{
public:
    Inode() {}
    virtual ~Inode() {}

private:
    std::list<DEntry*> dentries;
};

}
