/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once
 
 #include <cstddef>
 #include <cstdint>
 
 #include "utils/Elf64.h"
 #include "utils/Utils.h"

namespace kernel::fs {

namespace Elf64 = kernel::utils::Elf64;

class Elf64File
{
private:

public:
    using phdr_container = kernel::utils::SimpleContainer<Elf64::Phdr>;
    using shdr_container = kernel::utils::SimpleContainer<Elf64::Shdr>;

    Elf64File(std::uint64_t fileStartAddr)
    {
        _fileHeader = reinterpret_cast<Elf64::Ehdr*>(fileStartAddr);
        _programHeaders =
            phdr_container(reinterpret_cast<Elf64::Phdr*>(_fileHeader->e_phoff), _fileHeader->e_phnum, _fileHeader->e_phentsize);
        _sectionHeaders =
            shdr_container(reinterpret_cast<Elf64::Shdr*>(_fileHeader->e_shoff), _fileHeader->e_shnum, _fileHeader->e_shentsize);
    }

    phdr_container& getProgramHeaders()
    {
        return _programHeaders;
    }

    shdr_container& getSectionHeaders()
    {
        return _sectionHeaders;
    }

private:
    Elf64::Ehdr* _fileHeader;
    phdr_container _programHeaders;
    shdr_container _sectionHeaders;
};

}
