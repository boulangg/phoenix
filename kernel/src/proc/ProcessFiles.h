/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <vector>

#include "fs/File.h"

namespace kernel::proc {

class ProcessFiles
{
    fs::File* getFile(std::size_t fd);
    std::size_t registerFile(fs::File* file);
    void unregisterFile(std::size_t fd);

private:
    std::vector<fs::File*> _fds;
};

}
