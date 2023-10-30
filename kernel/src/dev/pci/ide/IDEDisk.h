/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include "dev/Disk.h"

namespace kernel::dev::pci::ide {

class IDEDisk : public Disk
{
public:
    IDEDisk(std::uint32_t major, std::uint32_t minor, std::string name);
    virtual ~IDEDisk() {}
};

}
