/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef __ELF64_HPP__
#define __ELF64_HPP__

#include <cstdint>

#include <mm/VirtualMapping.hpp>

class Elf64
{

	typedef uint64_t Elf64_Addr;
	typedef uint64_t Elf64_Off;
	typedef uint16_t Elf64_Half;
	typedef uint32_t Elf64_Word;
	typedef uint64_t Elf64_Xword;
	typedef int64_t  Elf64_Sxword;

	typedef struct
	{
		unsigned char e_ident[16]; 	/* ELF identification */
		Elf64_Half e_type; 			/* Object file type */
		Elf64_Half e_machine; 		/* Machine type */
		Elf64_Word e_version; 		/* Object file version */
		Elf64_Addr e_entry; 		/* Entry point address */
		Elf64_Off e_phoff; 			/* Program header offset */
		Elf64_Off e_shoff; 			/* Section header offset */
		Elf64_Word e_flags; 		/* Processor-specific flags */
		Elf64_Half e_ehsize; 		/* ELF header size */
		Elf64_Half e_phentsize; 	/* Size of program header entry */
		Elf64_Half e_phnum; 		/* Number of program header entries */
		Elf64_Half e_shentsize; 	/* Size of section header entry */
		Elf64_Half e_shnum; 		/* Number of section header entries */
		Elf64_Half e_shstrndx; 		/* Section name string table index */
	} Elf64_Ehdr;

	enum ident
	{
		EI_MAG0 = 0, 		// File identification
		EI_MAG1 = 1,
		EI_MAG2 = 2,
		EI_MAG3 = 3,
		EI_CLASS = 4, 		// File class
		EI_DATA = 5,  		// Data encoding
		EI_VERSION = 6, 	// File version
		EI_OSABI = 7, 		// OS/ABI identification
		EI_ABIVERSION = 8, 	// ABI version
		EI_PAD = 9, 		// Start of padding bytes
		EI_NIDENT = 16	 	// Size of e_ident[]
	};

	enum magicValue
	{
		MAG0_VAL = '\x7f',
		MAG1_VAL = '7',
		MAG2_VAL = 'E',
		MAG3_VAL = 'F'
	};

	enum classValue
	{
		ELFCLASS32 = 1,		// 32-bits objects
		ELFCLASS64 = 2		// 64-bits objects
	};

	enum dataValue
	{
		ELFDATA2LSB = 1, 	// little-endian data structures
		ELFDATA2MSB = 2		// big-endian data structures
	};

	enum OSABIValue
	{
		ELFOSABI_SYSV = 0, 			// System V ABI
		ELFOSABI_HPUX = 1, 			// HP-UX operating system
		ELFOSABI_STANDALONE = 255	// Standalone (embedded) application
	};

	enum typeValue
	{
		ET_NONE = 0, 	// No file type
		ET_REL = 1, 	// Relocable object file
		ET_EXEC = 2, 	// Executable file
		ET_DYN = 3, 	// Shared object file
		ET_CORE = 4, 	// Core file
		ET_LOOS = 0xFE00, 	// Environment -specific use
		ET_HIOS = 0xFEFF,
		ET_LOPROC = 0xFF00, // Processor-specific use
		ET_HIPROC = 0xFFFF
	};

	enum specialSectionindex
	{
		SHN_UNDEF = 0, 			// Undefined or meaningless section reference
		SHN_LOPROC = 0xFF00, 	// Processor-specific use
		SHN_HIPROC = 0xFF1F,
		SHN_LOOS = 0xFF20, 		// Environment-specific use
		SHN_HIOS = 0xFF3F,
		SHN_ABS = 0xFFF1, 		// Absolute reference
		SHN_COMMON = 0xFFF2		// COMMON block
	};

	typedef struct
	{
		Elf64_Word sh_name; 		/* Section name */
		Elf64_Word sh_type; 		/* Section type */
		Elf64_Xword sh_flags; 		/* Section attributes */
		Elf64_Addr sh_addr; 		/* Virtual address in memory */
		Elf64_Off sh_offset; 		/* Offset in file */
		Elf64_Xword sh_size; 		/* Size of section */
		Elf64_Word sh_link; 		/* Link to other section */
		Elf64_Word sh_info; 		/* Miscellaneous information */
		Elf64_Xword sh_addralign; 	/* Address alignment boundary */
		Elf64_Xword sh_entsize; 	/* Size of entries, if section has table */
	} Elf64_Shdr;

	enum sectionType
	{
		SHT_NULL = 0, 		// Unused section header
		SHT_PROGBITS = 1, 	// Informations defined by the program
		SHT_SYMTAB = 2, 	// Linker symbol table
		SHT_STRAB = 3, 		// String table
		SHT_RELA = 4, 		// "Rela" type relocation entries
		SHT_HASH = 5, 		// Symbol hash table
		SHT_DYNAMIC = 6, 	// Dynamic linking table
		SHT_NOTE = 7, 		// Note information
		SHT_NOBITS = 8, 	// Uninitialized space
		SHT_REL = 9, 		// "Rel" type relocation entries
		SHT_SHLIB = 0xa, 	// Reserved
		SHT_DYNSYM = 0xb, 	// Dynamic loader symbol table
		SHT_INIT_ARRAY = 0xe,
		SHT_FINI_ARRAY = 0xe,
		SHT_LOOS = 0x60000000, 		// Environment-specific use
		SHT_HIOS = 0x6FFFFFFF,
		SHT_LOPROC = 0x70000000, 	// Processor-specific use
		SHT_HIPROC = 0x7FFFFFFF,
	};

	enum sectionFlag
	{
		SHF_WRITE = 0x1, 		// Contains writable data
		SHF_ALLOC = 0x2, 		// Section located in memory image of program
		SHF_EXECINSTR = 0x4, 	// Contains executable instructions
		SHF_MASKOS = 0x0F000000, 	// Environment-specific use
		SHF_MASKPROC = 0xF0000000, 	// Processor-specific use
	};

	typedef struct
	{
		Elf64_Word st_name; 	/* Symbol name */
		unsigned char st_info; 	/* Type and Binding attributes */
		unsigned char st_other; /* Reserved */
		Elf64_Half st_shndx; 	/* Section table index */
		Elf64_Addr st_value; 	/* Symbol value */
		Elf64_Xword st_size; 	/* Size of object (e.g., common) */
	} Elf64_Sym;

	enum symbolBindings
	{
		STD_LOCAL = 0, 		// Not visible outside the object file
		STD_GLOBAL = 1, 	// Global symbol, visible to all object files
		STD_WEAK = 2, 		// Global scope but with lower precedence than global symbols
		STD_LOOS = 10, 		// Environment-specific use
		STD_HIOS = 12,
		STD_LOPROC = 12, 	// Processor-specific use
		STD_HIPROc = 15,
	};

	enum SymbolType
	{
		STT_NOTYPE = 0, 	// No type specified
		STT_OBJECT = 1, 	// Data object
		STT_FUNC = 2, 		// Function entry point
		STT_SECTION = 3, 	// Symbol associated with a section
		STT_FILE = 4, 		// Source file associated wth object file
		STT_LOOS = 10, 		// Environment-specific use
		STT_HIOS = 12,
		STT_LOPROC = 13, 	// Processor-specific use
		STT_HIPROC = 15,
	};

#define ELF64_R_SYM(i)((i) >> 32)
#define ELF64_R_TYPE(i)((i) & 0xffffffffL)
#define ELF64_R_INFO(s, t)(((s) << 32) + ((t) & 0xffffffffL))

	typedef struct
	{
		Elf64_Addr r_offset; 	/* Address of reference */
		Elf64_Xword r_info; 	/* Symbol index and type of relocation */
	} Elf64_Rel;

	typedef struct
	{
		Elf64_Addr r_offset; 	/* Address of reference */
		Elf64_Xword r_info; 	/* Symbol index and type of relocation */
		Elf64_Sxword r_addend; 	/* Constant part of expression */
	} Elf64_Rela;

	typedef struct
	{
		Elf64_Word p_type; 		/* Type of segment */
		Elf64_Word p_flags; 	/* Segment attributes */
		Elf64_Off p_offset; 	/* Offset in file */
		Elf64_Addr p_vaddr; 	/* Virtual address in memory */
		Elf64_Addr p_paddr; 	/* Reserved */
		Elf64_Xword p_filesz; 	/* Size of segment in file */
		Elf64_Xword p_memsz; 	/* Size of segment in memory */
		Elf64_Xword p_align; 	/* Alignment of segment */
	} Elf64_Phdr;

	enum programType
	{
		PT_NULL = 0,		// Unused entry
		PT_LOAD = 1, 		// Loadable segment
		PT_DYNAMIC = 2, 	// Dynamic linking table
		PT_INTERP = 3, 		// Program interpreter path name
		PT_NOTE = 4, 		// Note sections
		PT_SHLIB = 5, 		// Reserved
		PT_PHDR = 6, 		// Program header table
		PT_LOOS = 0x60000000, 	// Environment-specific use
		PT_HIOS = 0x6FFFFFFF,
		PT_LOPROC = 0x70000000, // Processor-specific use
		PT_HIPROC = 0x7FFFFFFF,
	};

	enum programFlag
	{
		PF_X = 0x1,		// Execute permission
		PF_W = 0x2,		// Write permission
		PF_R = 0x4,		// Read permission
		PF_MASKOS = 0x00FF0000, 	// Environment-specific use
		PF_MASKPROC = 0xFF000000, 	// Processor-specific use
	};

	// TODO note section

	// TODO dynamic section

	// TODO Hash table

public:
	static VirtualMapping* getVirtualMapping(File* file);
};

#endif //__ELF64_HPP__
