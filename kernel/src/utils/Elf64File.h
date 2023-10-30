/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include <cstddef>
#include <cstdint>

#include "fs/File.h"

#include "Elf64.h"
#include "Utils.h"

namespace kernel::utils {

class Elf64File
{
private:
public:
    using phdr_container = kernel::utils::SimpleContainer<Elf64::Phdr>;
    using shdr_container = kernel::utils::SimpleContainer<Elf64::Shdr>;

    Elf64File(fs::File* file) : _file(file)
    {
        _file->llseek(0, SEEK_SET);
        _file->read(reinterpret_cast<char*>(&_fileHeader), sizeof(Elf64::Ehdr));
        _file->llseek(_fileHeader.e_shoff + _fileHeader.e_shstrndx * _fileHeader.e_shentsize, SEEK_SET);
        _file->read(reinterpret_cast<char*>(&_strTableHdr), sizeof(Elf64::Shdr));
    }

    const Elf64::Ehdr& getFileHeader()
    {
        return _fileHeader;
    }

    Elf64::Phdr getProgramHeader(std::size_t index)
    {
        Elf64::Phdr pHdr{};
        if (index > _fileHeader.e_shnum) {
            return pHdr;
        }

        _file->llseek(_fileHeader.e_phoff + index * _fileHeader.e_phentsize, SEEK_SET);
        _file->read(reinterpret_cast<char*>(&pHdr), sizeof(Elf64::Phdr));
        return pHdr;
    }

    Elf64::Shdr getSectionHeader(std::size_t index)
    {
        Elf64::Shdr sHdr{};
        if (index > _fileHeader.e_shnum) {
            return sHdr;
        }

        _file->llseek(_fileHeader.e_shoff + index * _fileHeader.e_shentsize, SEEK_SET);
        _file->read(reinterpret_cast<char*>(&sHdr), sizeof(Elf64::Shdr));
        return sHdr;
    }

    void getString(std::size_t index, char* buffer, std::size_t bufferSize)
    {
        _file->llseek(_strTableHdr.sh_offset + index, SEEK_SET);
        _file->read(buffer, bufferSize);
    }

private:
    fs::File* _file;
    Elf64::Ehdr _fileHeader;
    Elf64::Shdr _strTableHdr;
};

}
