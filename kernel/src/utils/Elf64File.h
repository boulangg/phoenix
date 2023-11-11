/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include <cstddef>
#include <cstdint>

#include "Elf64.h"
#include "Utils.h"

namespace kernel::utils {

class Elf64File
{
private:
public:
    using phdr_container = kernel::utils::SimpleContainer<Elf64::Phdr>;
    using shdr_container = kernel::utils::SimpleContainer<Elf64::Shdr>;

    Elf64File(std::uint64_t fileStartAddr)
    {
        _fileStartAddr = fileStartAddr;
        _fileHeader = reinterpret_cast<Elf64::Ehdr*>(fileStartAddr);
        Elf64::Phdr* pHdr_ptr = reinterpret_cast<Elf64::Phdr*>(fileStartAddr + _fileHeader->e_phoff);
        _programHeaders = phdr_container(pHdr_ptr, _fileHeader->e_phnum, _fileHeader->e_phentsize);
        Elf64::Shdr* sHdr_ptr = reinterpret_cast<Elf64::Shdr*>(fileStartAddr + _fileHeader->e_shoff);
        _sectionHeaders = shdr_container(sHdr_ptr, _fileHeader->e_shnum, _fileHeader->e_shentsize);
    }

    phdr_container& getProgramHeaders()
    {
        return _programHeaders;
    }

    shdr_container& getSectionHeaders()
    {
        return _sectionHeaders;
    }

    char* getStringTable()
    {
        Elf64::Shdr& hdr = _sectionHeaders[_fileHeader->e_shstrndx];
        std::uint64_t startSection = _fileStartAddr + hdr.sh_offset;
        return reinterpret_cast<char*>(startSection);
    }

private:
    std::uint64_t _fileStartAddr;
    Elf64::Ehdr* _fileHeader;
    phdr_container _programHeaders;
    shdr_container _sectionHeaders;
};

}
