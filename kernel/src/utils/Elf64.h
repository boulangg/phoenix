/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

 #include <cstdint>

namespace kernel::utils {

namespace Elf64 {

using Addr = std::uint64_t;
using Off = std::uint64_t;
using Half = std::uint16_t;
using Word = std::uint32_t;
using Xword = std::uint64_t;
using Sxword = std::int64_t;

struct Ehdr
{
    unsigned char e_ident[16]; /* ELF identification */
    Half e_type;               /* Object file type */
    Half e_machine;            /* Machine type */
    Word e_version;            /* Object file version */
    Addr e_entry;              /* Entry point address */
    Off e_phoff;               /* Program header offset */
    Off e_shoff;               /* Section header offset */
    Word e_flags;              /* Processor-specific flags */
    Half e_ehsize;             /* ELF header size */
    Half e_phentsize;          /* Size of program header entry */
    Half e_phnum;              /* Number of program header entries */
    Half e_shentsize;          /* Size of section header entry */
    Half e_shnum;              /* Number of section header entries */
    Half e_shstrndx;           /* Section name string table index */
};

struct Phdr
{
    Word p_type;    /* Type of segment */
    Word p_flags;   /* Segment attributes */
    Off p_offset;   /* Offset in file */
    Addr p_vaddr;   /* Virtual address in memory */
    Addr p_paddr;   /* Reserved */
    Xword p_filesz; /* Size of segment in file */
    Xword p_memsz;  /* Size of segment in memory */
    Xword p_align;  /* Alignment of segment */
};

struct ProgramType
{
    static constexpr std::uint32_t PT_NULL = 0;            // Unused entry
    static constexpr std::uint32_t PT_LOAD = 1;            // Loadable segment
    static constexpr std::uint32_t PT_DYNAMIC = 2;         // Dynamic linking table
    static constexpr std::uint32_t PT_INTERP = 3;          // Program interpreter path name
    static constexpr std::uint32_t PT_NOTE = 4;            // Note sections
    static constexpr std::uint32_t PT_SHLIB = 5;           // Reserved
    static constexpr std::uint32_t PT_PHDR = 6;            // Program header table
    static constexpr std::uint32_t PT_LOOS = 0x60000000;   // Environment-specific use (start)
    static constexpr std::uint32_t PT_HIOS = 0x6FFFFFFF;   // Environment-specific use (end)
    static constexpr std::uint32_t PT_LOPROC = 0x70000000; // Processor-specific use (start)
    static constexpr std::uint32_t PT_HIPROC = 0x7FFFFFFF; // Processor-specific use (end)
};

struct ProgramFlag
{
    static constexpr std::uint32_t PF_X = 0x1;               // Execute permission
    static constexpr std::uint32_t PF_W = 0x2;               // Write permission
    static constexpr std::uint32_t PF_R = 0x4;               // Read permission
    static constexpr std::uint32_t PF_MASKOS = 0x00FF0000;   // Environment-specific use
    static constexpr std::uint32_t PF_MASKPROC = 0xFF000000; // Processor-specific use
};

struct Shdr
{
    Word sh_name;       /* Section name */
    Word sh_type;       /* Section type */
    Xword sh_flags;     /* Section attributes */
    Addr sh_addr;       /* Virtual address in memory */
    Off sh_offset;      /* Offset in file */
    Xword sh_size;      /* Size of section */
    Word sh_link;       /* Link to other section */
    Word sh_info;       /* Miscellaneous information */
    Xword sh_addralign; /* Address alignment boundary */
    Xword sh_entsize;   /* Size of entries, if section has table */
};

}

}
