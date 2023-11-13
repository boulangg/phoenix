/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include "fs/File.h"

namespace kernel::fs::kernelfs {

class KernelFile : public File
{
public:
    KernelFile(std::uint64_t startAddr, std::size_t fileSize);

    // File Operations
    ssize_t read(char* buffer, std::size_t size) override;
    ssize_t write(char* buffer, std::size_t size) override;
    loff_t llseek(loff_t offset, std::int32_t origin) override;

private:
    std::uint64_t _startAddr;
    std::size_t _fileSize;
};

}
