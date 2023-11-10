/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include "mem/AddressSpace.h"
#include "utils/Elf64File.h"

namespace kernel::proc {
class Process
{
public:
    Process();

    void load(utils::Elf64File& file);

private:
    mem::AddressSpace _addressSpace;
};
}
