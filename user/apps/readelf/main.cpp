/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include <stdio.h>
#include <string>

#include <elf.h>
#include <string.h>

void print_header(std::string name, std::size_t padding)
{
    auto header = name + ":";
    printf("  %*s", -(int)padding, header.c_str());
}

void print_elf_header(const Elf64_Ehdr& fileHeader)
{
    std::size_t padding = 35;

    printf("ELF Header:\n");
    printf("  Magic:  ");
    for (size_t i = 0; i < EI_NIDENT; i++) {
        printf(" %2.2x", fileHeader.e_ident[i]);
    }
    printf("\n");

    print_header("Class", padding);
    switch (fileHeader.e_ident[EI_CLASS]) {
    case ELFCLASS32:
        printf("ELF32\n");
        break;
    case ELFCLASS64:
        printf("ELF64\n");
        break;
    default:
        printf("%x Unknown\n", fileHeader.e_ident[EI_CLASS]);
        break;
    }

    print_header("Data", padding);
    switch (fileHeader.e_ident[EI_DATA]) {
    case ELFDATA2LSB:
        printf("2's complement, little endian\n");
        break;
    case ELFDATA2MSB:
        printf("2's complement, big endian\n");
        break;
    default:
        printf("%u (unknown)\n", fileHeader.e_ident[EI_DATA]);
        break;
    }

    print_header("Version", padding);
    switch (fileHeader.e_version) {
    case EV_CURRENT:
        printf("%u (current)\n", fileHeader.e_version);
        break;
    default:
        printf("%u (unknown)\n", fileHeader.e_version);
        break;
    }

    print_header("OS/ABI", padding);
    switch (fileHeader.e_ident[EI_OSABI]) {
    case ELFOSABI_SYSV:
        printf("UNIX - System V\n");
        break;
    case ELFOSABI_HPUX:
    default:
        printf("%x Unknown\n", fileHeader.e_ident[EI_OSABI]);
        break;
    }

    print_header("ABI Version", padding);
    printf("%u\n", fileHeader.e_ident[EI_ABIVERSION]);

    print_header("Type", padding);
    switch (fileHeader.e_type) {
    case ET_DYN:
        printf("DYN (Position-Independent Executable file)\n");
        break;
    case ET_NONE:
    case ET_REL:
    case ET_EXEC:
    case ET_CORE:
    default:
        printf("%x Unknown\n", fileHeader.e_type);
        break;
    }

    print_header("Machine", padding);
    switch (fileHeader.e_type) {
    case 3:
        printf("Advanced Micro Devices X86-64\n");
        break;
    default:
        printf("%x Unknown\n", fileHeader.e_type);
        break;
    }

    print_header("Version", padding);
    printf("0x%x\n", fileHeader.e_version);
    print_header("Entry point address", padding);
    printf("0x%lx\n", fileHeader.e_entry);
    print_header("Start of program headers", padding);
    printf("%lu (bytes into file)\n", fileHeader.e_phoff);
    print_header("Start of section headers", padding);
    printf("%lu (bytes into file)\n", fileHeader.e_shoff);
    print_header("Flags", padding);
    printf("0x%x\n", fileHeader.e_flags);
    print_header("Size of this header", padding);
    printf("%u (bytes)\n", fileHeader.e_ehsize);
    print_header("Size of program headers", padding);
    printf("%u (bytes)\n", fileHeader.e_phentsize);
    print_header("Number of program headers", padding);
    printf("%u\n", fileHeader.e_phnum);
    print_header("Size of section headers", padding);
    printf("%u (bytes)\n", fileHeader.e_shentsize);
    print_header("Number of section headers", padding);
    printf("%u\n", fileHeader.e_shnum);
    print_header("Section header string table index", padding);
    printf("%u\n", fileHeader.e_shstrndx);
}

void print_program_header(FILE* fd, const Elf64_Phdr& ph)
{
    const char* type = "";
    switch (ph.p_type) {
    case PT_NULL:
        type = "NULL";
        break;
    case PT_LOAD:
        type = "LOAD";
        break;
    case PT_DYNAMIC:
        type = "DYNAMIC";
        break;
    case PT_INTERP:
        type = "INTERP";
        break;
    case PT_NOTE:
        type = "NOTE";
        break;
    case PT_SHLIB:
        type = "SHLIB";
        break;
    case PT_PHDR:
        type = "PHDR";
        break;
    case PT_TLS:
        type = "TLS";
        break;
    case PT_GNU_EH_FRAME:
        type = "GNU_EH_FRAME";
        break;
    case PT_GNU_STACK:
        type = "GNU_STACK";
        break;
    case PT_GNU_RELRO:
        type = "GNU_RELRO";
        break;
    case PT_GNU_PROPERTY:
        type = "GNU_PROPERTY";
        break;
    default:
        type = "UNKNOWN";
        break;
    }
    printf("  %-14s 0x%016lx 0x%016lx 0x%016lx\n", type, ph.p_offset, ph.p_vaddr, ph.p_paddr);

    std::string flags = "";
    if (ph.p_flags & PF_R) {
        flags += "R";
    } else {
        flags += " ";
    }
    if (ph.p_flags & PF_W) {
        flags += "W";
    } else {
        flags += " ";
    }
    if (ph.p_flags & PF_X) {
        flags += "E";
    } else {
        flags += " ";
    }
    printf("  %-14c 0x%016lx 0x%016lx  %-6s 0x%-4lx\n", ' ', ph.p_filesz, ph.p_memsz, flags.c_str(), ph.p_align);

    if (ph.p_type == PT_INTERP) {
        char* interpStr = new char[ph.p_filesz];
        fseek(fd, ph.p_offset, SEEK_SET);
        fread((void*)interpStr, ph.p_filesz, 1, fd);
        printf("      [Requesting program interpreter: %s]\n", interpStr);
        delete[] interpStr;
    }
}

void print_program_headers(FILE* fd, const Elf64_Ehdr& fileHeader)
{
    printf("Program Headers:\n");
    printf("  %-14s %-18s %-18s %-18s\n", "Type", "Offset", "VirtAddr", "PhysAddr");
    printf("  %-14c %-18s %-18s  %-6s %-6s\n", ' ', "FileSiz", "MemSiz", "Flags", "Align");
    for (std::size_t i = 0; i < fileHeader.e_phnum; ++i) {
        Elf64_Phdr programHeader;
        fseek(fd, fileHeader.e_phoff + sizeof(Elf64_Phdr) * i, SEEK_SET);
        fread((void*)&programHeader, sizeof(Elf64_Phdr), 1, fd);
        print_program_header(fd, programHeader);
    }
}

#ifdef SHT_GNU_verdef
#define SHT_GNU_VERDEF SHT_GNU_verdef
#endif

#ifdef SHT_GNU_versym
#define SHT_GNU_VERSYM SHT_GNU_versym
#endif

#ifdef SHT_GNU_verneed
#define SHT_GNU_VERNEED SHT_GNU_verneed
#endif

void print_section_header(const std::size_t id, const Elf64_Shdr& sh, const char* shstr)
{
    const char* type = "";
    switch (sh.sh_type) {
    case SHT_NULL:
        type = "NULL";
        break;
    case SHT_PROGBITS:
        type = "PROGBITS";
        break;
    case SHT_SYMTAB:
        type = "SYMTAB";
        break;
    case SHT_STRTAB:
        type = "STRTAB";
        break;
    case SHT_RELA:
        type = "RELA";
        break;
    case SHT_HASH:
        type = "HASH";
        break;
    case SHT_DYNAMIC:
        type = "DYNAMIC";
        break;
    case SHT_NOTE:
        type = "NOTE";
        break;
    case SHT_NOBITS:
        type = "NOBITS";
        break;
    case SHT_REL:
        type = "REL";
        break;
    case SHT_SHLIB:
        type = "SHLIB";
        break;
    case SHT_DYNSYM:
        type = "DYNSYM";
        break;
    case SHT_GNU_HASH:
        type = "GNU_HASH";
        break;
    case SHT_GNU_VERDEF:
        type = "VERDEF";
        break;
    case SHT_GNU_VERSYM:
        type = "VERSYM";
        break;
    case SHT_GNU_VERNEED:
        type = "VERNEED";
        break;
    default:
        type = "UNKNOWN";
        break;
    }
    printf("  [%2lu] %-17.17s %-16s %016lx  %08lx\n", id, shstr + sh.sh_name, type, sh.sh_addr, sh.sh_offset);

    std::string flags = "";
    if (sh.sh_flags & SHF_WRITE) {
        flags += "W";
    }
    if (sh.sh_flags & SHF_ALLOC) {
        flags += "A";
    }
    if (sh.sh_flags & SHF_EXECINSTR) {
        flags += "X";
    }
    if (sh.sh_flags & SHF_MERGE) {
        flags += "M";
    }
    if (sh.sh_flags & SHF_STRINGS) {
        flags += "S";
    }
    if (sh.sh_flags & SHF_INFO_LINK) {
        flags += "I";
    }
    if (sh.sh_flags & SHF_LINK_ORDER) {
        flags += "L";
    }
    if (sh.sh_flags & SHF_OS_NONCONFORMING) {
        flags += "O";
    }
    if (sh.sh_flags & SHF_GROUP) {
        flags += "G";
    }
    if (sh.sh_flags & SHF_TLS) {
        flags += "T";
    }
    if (sh.sh_flags & SHF_COMPRESSED) {
        flags += "C";
    }
    if (sh.sh_flags & SHF_MASKOS) {
        flags += "o";
    }
    if (sh.sh_flags & SHF_MASKPROC) {
        flags += "p";
    }
    printf("       %016lx  %016lx %3s    %4u %5u     %-2lu\n", sh.sh_size, sh.sh_entsize, flags.c_str(), sh.sh_link,
           sh.sh_info, sh.sh_addralign);
}

void print_section_headers(FILE* fd, const Elf64_Ehdr& fileHeader)
{
    // Load section name string table
    Elf64_Shdr shstrSectionHeader;
    fseek(fd, fileHeader.e_shoff + sizeof(Elf64_Shdr) * fileHeader.e_shstrndx, SEEK_SET);
    fread((void*)&shstrSectionHeader, sizeof(Elf64_Shdr), 1, fd);
    char* shstr = new char[shstrSectionHeader.sh_size];
    fseek(fd, shstrSectionHeader.sh_offset, SEEK_SET);
    fread((void*)shstr, shstrSectionHeader.sh_size, 1, fd);

    printf("Section Headers:\n");
    printf("  [Nr] %-17s %-16s %-16s  %-8s\n", "Name", "Type", "Address", "Offset");
    printf("       %-17s %-16s %-6s %-5s %-5s %-6s\n", "Size", "EntSize", "Flags", "Link", "Info", "Align");
    for (std::size_t i = 0; i < fileHeader.e_shnum; ++i) {
        Elf64_Shdr sectionHeader;
        fseek(fd, fileHeader.e_shoff + sizeof(Elf64_Shdr) * i, SEEK_SET);
        fread((void*)&sectionHeader, sizeof(Elf64_Shdr), 1, fd);
        print_section_header(i, sectionHeader, shstr);
    }
    printf("Key to Flags:\n");
    printf("  W (write), A (alloc), X (execute), M (merge), S (strings), I (info),\n");
    printf("  L (link order), O (extra OS processing required), G (group), T (TLS),\n");
    printf("  C (compressed), x (unknown), o (OS specific), E (exclude),\n");
    printf("  D (mbind), l (large), p (processor specific)\n");

    delete[] shstr;
}

Elf64_Shdr find_section_header(FILE* fd, const Elf64_Ehdr& fileHeader, const char* sectionName)
{
    // Load section name string table
    Elf64_Shdr shstrSectionHeader;
    fseek(fd, fileHeader.e_shoff + sizeof(Elf64_Shdr) * fileHeader.e_shstrndx, SEEK_SET);
    fread((void*)&shstrSectionHeader, sizeof(Elf64_Shdr), 1, fd);
    char* shstr = new char[shstrSectionHeader.sh_size];
    fseek(fd, shstrSectionHeader.sh_offset, SEEK_SET);
    fread((void*)shstr, shstrSectionHeader.sh_size, 1, fd);

    for (std::size_t i = 0; i < fileHeader.e_shnum; ++i) {
        Elf64_Shdr sectionHeader;
        fseek(fd, fileHeader.e_shoff + sizeof(Elf64_Shdr) * i, SEEK_SET);
        fread((void*)&sectionHeader, sizeof(Elf64_Shdr), 1, fd);
        if (strcmp(shstr + sectionHeader.sh_name, sectionName) == 0) {
            return sectionHeader;
        }
    }

    Elf64_Shdr sectionHeader;
    return sectionHeader;
}

#ifndef Elf64_Sym_Type
#define Elf64_Sym_Type(sym) (sym.st_info & 0x0F)
#endif

#ifndef Elf64_Sym_Bind
#define Elf64_Sym_Bind(sym) (sym.st_info >> 4)
#endif

#ifndef STT_IFUNC
#define STT_IFUNC 10
#endif

void print_symbol(const std::size_t id, const Elf64_Sym& sym, const char* dynstr)
{
    const char* binding = "";
    switch (Elf64_Sym_Bind(sym)) {
    case STB_LOCAL:
        binding = "LOCAL";
        break;
    case STB_GLOBAL:
        binding = "GLOBAL";
        break;
    case STB_WEAK:
        binding = "WEAK";
        break;
    default:
        binding = "UNKNOWN";
        break;
    }

    const char* type = "";
    switch (Elf64_Sym_Type(sym)) {
    case STT_NOTYPE:
        type = "NOTYPE";
        break;
    case STT_OBJECT:
        type = "OBJECT";
        break;
    case STT_FUNC:
        type = "FUNC";
        break;
    case STT_SECTION:
        type = "SECTION";
        break;
    case STT_FILE:
        type = "FILE";
        break;
    case STT_COMMON:
        type = "COMMON";
        break;
    case STT_TLS:
        type = "TLS";
        break;
    case STT_IFUNC:
        type = "IFUNC";
        break;
    default:
        type = "UNKNOWN";
        break;
    }

    printf(" %5lu: %016lx %5lu %-7s %-6s DEFAULT  %3i %s\n", id, sym.st_value, sym.st_size, type, binding, sym.st_shndx,
           dynstr + sym.st_name);
}

void print_dynsym_section(FILE* fd, const Elf64_Ehdr& fileHeader)
{
    Elf64_Shdr dynsymShdr = find_section_header(fd, fileHeader, ".dynsym");

    // Load symbol name string table
    Elf64_Shdr dynstrShdr = find_section_header(fd, fileHeader, ".dynstr");
    char* dynstr = new char[dynstrShdr.sh_size];
    fseek(fd, dynstrShdr.sh_offset, SEEK_SET);
    fread((void*)dynstr, dynstrShdr.sh_size, 1, fd);

    printf("Symbol table: \n");
    printf("   Num:    Value          Size Type    Bind   Vis      Ndx Name\n");
    size_t dynSymNum = dynsymShdr.sh_size / dynsymShdr.sh_entsize;
    for (size_t i = 0; i < dynSymNum; i++) {
        Elf64_Sym sym;
        fseek(fd, dynsymShdr.sh_offset + sizeof(Elf64_Sym) * i, SEEK_SET);
        fread((void*)&sym, sizeof(Elf64_Sym), 1, fd);
        print_symbol(i, sym, dynstr);
    }
}

#define DYNAMIC_BUFFER_SZ 256

void print_dynamic(const Elf64_Dyn& dyn)
{
    const char* type = "";
    char value[DYNAMIC_BUFFER_SZ];
    value[DYNAMIC_BUFFER_SZ - 1] = '\0';
    snprintf(value, DYNAMIC_BUFFER_SZ - 1, "0x%lx", dyn.d_un.d_ptr);
    switch (dyn.d_tag) {
    case DT_NULL:
        type = "(NULL)";
        break;
    case DT_NEEDED:
        type = "(NEEDED)";
        // snprintf(value, DYNAMIC_BUFFER_SZ - 1, "%ui (bytes)", dyn.d_un.d_val);
        break;
    case DT_PLTRELSZ:
        type = "(PLTRELSZ)";
        snprintf(value, DYNAMIC_BUFFER_SZ - 1, "%lu (bytes)", dyn.d_un.d_val);
        break;
    case DT_PLTGOT:
        type = "(PLTGOT)";
        break;
    case DT_HASH:
        type = "(HASH)";
        break;
    case DT_STRTAB:
        type = "(STRTAB)";
        break;
    case DT_SYMTAB:
        type = "(SYMTAB)";
        break;
    case DT_RELA:
        type = "(RELA)";
        break;
    case DT_RELASZ:
        type = "(RELASZ)";
        break;
    case DT_RELAENT:
        type = "(RELAENT)";
        snprintf(value, DYNAMIC_BUFFER_SZ - 1, "%lu (bytes)", dyn.d_un.d_val);
        break;
    case DT_STRSZ:
        type = "(STRSZ)";
        snprintf(value, DYNAMIC_BUFFER_SZ - 1, "%lu (bytes)", dyn.d_un.d_val);
        break;
    case DT_SYMENT:
        type = "(SYMENT)";
        snprintf(value, DYNAMIC_BUFFER_SZ - 1, "%lu (bytes)", dyn.d_un.d_val);
        break;
    case DT_INIT:
        type = "(INIT)";
        break;
    case DT_FINI:
        type = "(FINI)";
        break;
    case DT_SONAME:
        type = "(SONAME)";
        break;
    case DT_RPATH:
        type = "(RPATH)";
        break;
    case DT_SYMBOLIC:
        type = "(SYMBOLIC)";
        break;
    case DT_REL:
        type = "(REL)";
        break;
    case DT_RELSZ:
        type = "(RELSZ)";
        break;
    case DT_RELENT:
        type = "(RELENT)";
        break;
    case DT_PLTREL:
        type = "(PLTREL)";
        if (dyn.d_un.d_val == DT_REL) {
            snprintf(value, DYNAMIC_BUFFER_SZ - 1, "REL");
        } else if (dyn.d_un.d_val == DT_RELA) {
            snprintf(value, DYNAMIC_BUFFER_SZ - 1, "RELA");
        } else {
            snprintf(value, DYNAMIC_BUFFER_SZ - 1, "0x%lx (UNKNOWN)", dyn.d_un.d_ptr);
        }
        break;
    case DT_DEBUG:
        type = "(DEBUG)";
        break;
    case DT_TEXTREL:
        type = "(TEXTREL)";
        break;
    case DT_JMPREL:
        type = "(JMPREL)";
        break;
    case DT_BIND_NOW:
        type = "(BIND_NOW)";
        break;
    case DT_INIT_ARRAY:
        type = "(INIT_ARRAY)";
        break;
    case DT_FINI_ARRAY:
        type = "(FINI_ARRAY)";
        break;
    case DT_INIT_ARRAYSZ:
        type = "(INIT_ARRAYSZ)";
        break;
    case DT_FINI_ARRAYSZ:
        type = "(FINI_ARRAYSZ)";
        break;
    case DT_RUNPATH:
        type = "(RUNPATH)";
        break;
    case DT_FLAGS:
        type = "(FLAGS)";
        switch (dyn.d_un.d_val) {
        case DF_ORIGIN:
            snprintf(value, DYNAMIC_BUFFER_SZ - 1, "ORIGIN");
            break;
        case DF_SYMBOLIC:
            snprintf(value, DYNAMIC_BUFFER_SZ - 1, "SYMBOLIC");
            break;
        case DF_TEXTREL:
            snprintf(value, DYNAMIC_BUFFER_SZ - 1, "TEXTREL");
            break;
        case DF_BIND_NOW:
            snprintf(value, DYNAMIC_BUFFER_SZ - 1, "BIND_NOW");
            break;
        case DF_STATIC_TLS:
            snprintf(value, DYNAMIC_BUFFER_SZ - 1, "STATIC_TLS");
            break;
        default:
            snprintf(value, DYNAMIC_BUFFER_SZ - 1, "0x%lx (UNKNOWN)", dyn.d_un.d_ptr);
            break;
        }
        break;
    case DT_PREINIT_ARRAY:
        type = "(PREINIT_ARRAY)";
        break;
    case DT_PREINIT_ARRAYSZ:
        type = "(PREINIT_ARRAYSZ)";
        break;
    case DT_SYMTAB_SHNDX:
        type = "(SYMTAB_SHNDX)";
        break;
    case DT_CHECKSUM:
        type = "(CHECKSUM)";
        snprintf(value, DYNAMIC_BUFFER_SZ - 1, "%lu", dyn.d_un.d_val);
        break;
    case DT_PLTPADSZ:
        type = "(PLTPADSZ)";
        snprintf(value, DYNAMIC_BUFFER_SZ - 1, "%lu", dyn.d_un.d_val);
        break;
    case DT_MOVEENT:
        type = "(MOVEENT)";
        snprintf(value, DYNAMIC_BUFFER_SZ - 1, "%lu", dyn.d_un.d_val);
        break;
    case DT_MOVESZ:
        type = "(MOVESZ)";
        snprintf(value, DYNAMIC_BUFFER_SZ - 1, "%lu", dyn.d_un.d_val);
        break;
    case DT_POSFLAG_1:
        type = "(POSFLAG_1)";
        snprintf(value, DYNAMIC_BUFFER_SZ - 1, "%lu", dyn.d_un.d_val);
        break;
    case DT_SYMINSZ:
        type = "(SYMINSZ)";
        snprintf(value, DYNAMIC_BUFFER_SZ - 1, "%lu", dyn.d_un.d_val);
        break;
    case DT_SYMINENT:
        type = "(SYMINENT)";
        snprintf(value, DYNAMIC_BUFFER_SZ - 1, "%lu", dyn.d_un.d_val);
        break;
    case DT_GNU_HASH:
        type = "(GNU_HASH)";
        break;
    case DT_CONFIG:
        type = "(CONFIG)";
        break;
    case DT_DEPAUDIT:
        type = "(DEPAUDIT)";
        break;
    case DT_AUDIT:
        type = "(AUDIT)";
        break;
    case DT_PLTPAD:
        type = "(PLTPAD)";
        break;
    case DT_MOVETAB:
        type = "(MOVETAB)";
        break;
    case DT_SYMINFO:
        type = "(SYMINFO)";
        break;
    case DT_VERSYM:
        type = "(VERSYM)";
        snprintf(value, DYNAMIC_BUFFER_SZ - 1, "%lu", dyn.d_un.d_val);
        break;
    case DT_RELACOUNT:
        type = "(RELACOUNT)";
        snprintf(value, DYNAMIC_BUFFER_SZ - 1, "%lu", dyn.d_un.d_val);
        break;
    case DT_RELCOUNT:
        type = "(RELACOUNT)";
        snprintf(value, DYNAMIC_BUFFER_SZ - 1, "%lu", dyn.d_un.d_val);
        break;
    case DT_FLAGS_1:
        type = "(FLAGS_1)";
        {
            std::string flags1 = "Flags:";
            if (dyn.d_un.d_val & DF_1_NOW) {
                flags1 += " NOW";
            }
            if (dyn.d_un.d_val & DF_1_GLOBAL) {
                flags1 += " GLOBAL";
            }
            if (dyn.d_un.d_val & DF_1_GROUP) {
                flags1 += " GROUP";
            }
            if (dyn.d_un.d_val & DF_1_NODELETE) {
                flags1 += " NODELETE";
            }
            if (dyn.d_un.d_val & DF_1_LOADFLTR) {
                flags1 += " LOADFLTR";
            }
            if (dyn.d_un.d_val & DF_1_INITFIRST) {
                flags1 += " INITFIRST";
            }
            if (dyn.d_un.d_val & DF_1_NOOPEN) {
                flags1 += " NOOPEN";
            }
            if (dyn.d_un.d_val & DF_1_ORIGIN) {
                flags1 += " ORIGIN";
            }
            if (dyn.d_un.d_val & DF_1_DIRECT) {
                flags1 += " DIRECT";
            }
            if (dyn.d_un.d_val & DF_1_INTERPOSE) {
                flags1 += " INTERPOSE";
            }
            if (dyn.d_un.d_val & DF_1_NODEFLIB) {
                flags1 += " NODEFLIB";
            }
            if (dyn.d_un.d_val & DF_1_NODUMP) {
                flags1 += " NODUMP";
            }
            if (dyn.d_un.d_val & DF_1_CONFALT) {
                flags1 += " CONFALT";
            }
            if (dyn.d_un.d_val & DF_1_ENDFILTEE) {
                flags1 += " ENDFILTEE";
            }
            if (dyn.d_un.d_val & DF_1_DISPRELDNE) {
                flags1 += " DISPRELDNE";
            }
            if (dyn.d_un.d_val & DF_1_DISPRELPND) {
                flags1 += " DISPRELPND";
            }
            if (dyn.d_un.d_val & DF_1_NODIRECT) {
                flags1 += " NODIRECT";
            }
            if (dyn.d_un.d_val & DF_1_IGNMULDEF) {
                flags1 += " IGNMULDEF";
            }
            if (dyn.d_un.d_val & DF_1_NOKSYMS) {
                flags1 += " NOKSYMS";
            }
            if (dyn.d_un.d_val & DF_1_NOHDR) {
                flags1 += " NOHDR";
            }
            if (dyn.d_un.d_val & DF_1_EDITED) {
                flags1 += " EDITED";
            }
            if (dyn.d_un.d_val & DF_1_NORELOC) {
                flags1 += " NORELOC";
            }
            if (dyn.d_un.d_val & DF_1_SYMINTPOSE) {
                flags1 += " SYMINTPOSE";
            }
            if (dyn.d_un.d_val & DF_1_GLOBAUDIT) {
                flags1 += " GLOBAUDIT";
            }
            if (dyn.d_un.d_val & DF_1_SINGLETON) {
                flags1 += " SINGLETON";
            }
            if (dyn.d_un.d_val & DF_1_SINGLETON) {
                flags1 += " SINGLETON";
            }
            if (dyn.d_un.d_val & DF_1_PIE) {
                flags1 += " PIE";
            }
            snprintf(value, DYNAMIC_BUFFER_SZ - 1, "%s", flags1.c_str());
        }
        // snprintf(value, DYNAMIC_BUFFER_SZ - 1, "%lx", dyn.d_un.d_val);
        break;
    case DT_VERDEF:
        type = "(VERNEED)";
        break;
    case DT_VERDEFNUM:
        type = "(DT_VERDEFNUM)";
        snprintf(value, DYNAMIC_BUFFER_SZ - 1, "%lu", dyn.d_un.d_val);
        break;
    case DT_VERNEED:
        type = "(VERNEED)";
        break;
    case DT_VERNEEDNUM:
        type = "(VERNEEDNUM)";
        snprintf(value, DYNAMIC_BUFFER_SZ - 1, "%lu", dyn.d_un.d_val);
        break;
    default:
        type = "(UNKNOWN)";
        break;
    }
    printf(" 0x%016lx %-20s %s\n", dyn.d_tag, type, value);
}

void print_dynamic_section(FILE* fd, const Elf64_Ehdr& fileHeader)
{
    Elf64_Shdr dynamicShdr = find_section_header(fd, fileHeader, ".dynamic");

    printf("Dynamic section: \n");
    printf("  Tag                Type                 Name/Value\n");
    size_t dynamicNum = dynamicShdr.sh_size / dynamicShdr.sh_entsize;
    for (size_t i = 0; i < dynamicNum; i++) {
        Elf64_Dyn dyn;
        fseek(fd, dynamicShdr.sh_offset + sizeof(Elf64_Dyn) * i, SEEK_SET);
        fread((void*)&dyn, sizeof(Elf64_Dyn), 1, fd);
        print_dynamic(dyn);
    }
}

int main()
{
    std::string _filename = "/usr/bin/cat";
    auto fd = fopen(_filename.c_str(), "rb");

    Elf64_Ehdr fileHeader;

    fread((void*)&fileHeader, sizeof(Elf64_Ehdr), 1, fd);
    print_elf_header(fileHeader);
    printf("\n");
    print_program_headers(fd, fileHeader);
    printf("\n");
    print_section_headers(fd, fileHeader);
    printf("\n");
    print_dynsym_section(fd, fileHeader);
    printf("\n");
    print_dynamic_section(fd, fileHeader);
}
