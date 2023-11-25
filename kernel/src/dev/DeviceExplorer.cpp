/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #include "DeviceExplorer.h"

#include "pci/PCIManager.h"

namespace kernel::dev {

DeviceExplorer::DeviceExplorer() {}

void DeviceExplorer::init()
{
    pci::PCIManager::initPCI();
}

}
