/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include <list>

#include "Disk.h"

namespace kernel::dev {

class DeviceExplorer
{
public:
    DeviceExplorer();

    void init();
    void addDisk(Disk* disk);
    void addDrive(BlockDevice* disk);
    void blockDeviceService();

private:
    std::list<BlockDevice*> _blkDevs;
};
}
