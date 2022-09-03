#pragma once

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// Data representation

	typedef uint64_t	Elf64_Addr;
	typedef uint64_t	Elf64_Off;
	typedef uint16_t	Elf64_Half;
	typedef uint32_t	Elf64_Word;
	typedef uint64_t	Elf64_Xword;
	typedef int64_t		Elf64_Sxword;

	// File header

	enum ident
	{
		EI_MAG0 = 0, 		/* File identification */
		EI_MAG1 = 1,
		EI_MAG2 = 2,
		EI_MAG3 = 3,
		EI_CLASS = 4, 		/* File class */
		EI_DATA = 5,  		/* Data encoding */
		EI_VERSION = 6, 	/* File version */
		EI_OSABI = 7, 		/* OS/ABI identification */
		EI_ABIVERSION = 8, 	/* ABI version */
		EI_PAD = 9, 		/* Start of padding bytes */
		EI_NIDENT = 16		/* Size of e_ident[] */
	};

	enum magicValue
	{
		MAG0_VAL = '\x7f',
		MAG1_VAL = 'E',
		MAG2_VAL = 'L',
		MAG3_VAL = 'F'
	};

	enum classValue
	{
		ELFCLASS32 = 1,		/* 32-bits objects */
		ELFCLASS64 = 2		/* 64-bits objects */
	};

	enum dataValue
	{
		ELFDATA2LSB = 1, 	/* little-endian data structures */
		ELFDATA2MSB = 2		/* big-endian data structures */
	};

	enum versionValue : Elf64_Word
	{
		EV_CURRENT = 1
	};

	enum OSABIValue
	{
		ELFOSABI_SYSV = 0, 			/* System V ABI */
		ELFOSABI_HPUX = 1, 			/* HP-UX operating system */
		ELFOSABI_STANDALONE = 255	/* Standalone (embedded) application */
	};

	enum sysvABIVersionValue
	{
		ELFSYSVABIVERSION = 0
	};

	enum typeValue : Elf64_Half
	{
		ET_NONE = 0, 		/* No file type */
		ET_REL = 1, 		/* Relocable object file */
		ET_EXEC = 2, 		/* Executable file */
		ET_DYN = 3, 		/* Shared object file */
		ET_CORE = 4, 		/* Core file */
		ET_LOOS = 0xFE00, 	/* Environment -specific use */
		ET_HIOS = 0xFEFF,
		ET_LOPROC = 0xFF00, /* Processor-specific use */
		ET_HIPROC = 0xFFFF
	};

	typedef struct
	{
		unsigned char 	e_ident[EI_NIDENT]; /* ELF identification */
		typeValue 		e_type; 			/* Object file type */
		Elf64_Half 		e_machine; 			/* Machine type */
		versionValue 	e_version; 			/* Object file version */
		Elf64_Addr 		e_entry; 			/* Entry point address */
		Elf64_Off 		e_phoff; 			/* Program header offset */
		Elf64_Off 		e_shoff; 			/* Section header offset */
		Elf64_Word 		e_flags; 			/* Processor-specific flags */
		Elf64_Half 		e_ehsize; 			/* ELF header size */
		Elf64_Half 		e_phentsize; 		/* Size of program header entry */
		Elf64_Half 		e_phnum; 			/* Number of program header entries */
		Elf64_Half 		e_shentsize; 		/* Size of section header entry */
		Elf64_Half 		e_shnum; 			/* Number of section header entries */
		Elf64_Half 		e_shstrndx; 		/* Section name string table index */
	} Elf64_Ehdr;

	// Sections

	enum specialSectionindex
	{
		SHN_UNDEF = 0, 			/* Undefined or meaningless section reference */
		SHN_LOPROC = 0xFF00, 	/* Processor-specific use */
		SHN_HIPROC = 0xFF1F,
		SHN_LOOS = 0xFF20, 		/* Environment-specific use */
		SHN_HIOS = 0xFF3F,
		SHN_ABS = 0xFFF1, 		/* Absolute reference */
		SHN_COMMON = 0xFFF2		/* COMMON block */
	};

	enum sectionType : Elf64_Word
	{
		SHT_NULL = 0, 					/* Unused section header */
		SHT_PROGBITS = 1, 				/* Informations defined by the program */
		SHT_SYMTAB = 2, 				/* Linker symbol table */
		SHT_STRTAB = 3, 				/* String table */
		SHT_RELA = 4, 					/* "Rela" type relocation entries */
		SHT_HASH = 5, 					/* Symbol hash table */
		SHT_DYNAMIC = 6, 				/* Dynamic linking table */
		SHT_NOTE = 7, 					/* Note information */
		SHT_NOBITS = 8, 				/* Uninitialized space */
		SHT_REL = 9, 					/* "Rel" type relocation entries */
		SHT_SHLIB = 0xa, 				/* Reserved */
		SHT_DYNSYM = 0xb, 				/* Dynamic loader symbol table */
		SHT_INIT_ARRAY = 0xe,
		SHT_FINI_ARRAY = 0xe,
		SHT_LOOS = 0x60000000, 			/* Environment-specific use */
		SHT_GNU_HASH = 0x6ffffff6,		/* GNU hash */
		SHT_GNU_VERDEF = 0x6ffffffd,	/* GNU version definition */
		SHT_GNU_VERSYM = 0x6ffffffe,	/* GNU version symbol */
		SHT_GNU_VERNEED = 0x6fffffff,	/* GNU version needed */
		SHT_HIOS = 0x6FFFFFFF,
		SHT_LOPROC = 0x70000000, 		/* Processor-specific use */
		SHT_HIPROC = 0x7FFFFFFF,
	};

	enum sectionFlag : Elf64_Xword
	{
		SHF_WRITE = 0x1, 				/* Contains writable data */
		SHF_ALLOC = 0x2, 				/* Section located in memory image of program */
		SHF_EXECINSTR = 0x4, 			/* Contains executable instructions */
		SHF_MERGE = 0x10,				/* The data in the section may be merged to eliminate duplication. */
		SHF_STRINGS = 0x20,				/* Contains null-terminated character strings. */
		SHF_INFO_LINK = 0x40,			/* The sh_info field of this section header holds a section header table index. */
		SHF_LINK_ORDER = 0x80,			/* This flag adds special ordering requirements for link editors. */
		SHF_OS_NONCONFORMING = 0x100,	/* This section requires special OS-specific processing (beyond the standard linking rules) to avoid incorrect behavior. */
		SHF_GROUP = 0x200,				/* This section is a member (perhaps the only one) of a section group. */
		SHF_TLS = 0x400,				/* This section holds Thread-Local Storage. */
		SHF_COMPRESSED = 0x800,			/* section containing compressed data. */
		SHF_MASKOS = 0x0F000000, 		/* Environment-specific use */
		SHF_MASKPROC = 0xF0000000 		/* Processor-specific use */
	};

	enum section
	{
		GRP_COMDAT = 0x1,
		GRP_MASKOS = 0x0ff00000,
		GRP_MASKPROC = 0xf0000000
	};

	enum compressionType : Elf64_Word
	{
		ELFCOMPRESS_ZLIB = 1,				/* ZLIB algoritm */
		ELFCOMPRESS_LOOS = 0x60000000,		/* Operating system-specific semantics */
		ELFCOMPRESS_HIOS = 0x6fffffff,
		ELFCOMPRESS_LOPROC = 0x70000000,	/* Processor-specific use */
		ELFCOMPRESS_HIPROC = 0x7fffffff
	};

	typedef struct
	{
		compressionType	ch_type;		/* Compression algorithm */
		Elf64_Word	ch_reserved;
		Elf64_Xword	ch_size;			/* Size in bytes of the uncompressed data */
		Elf64_Xword	ch_addralign;		/* Required alignment for the uncompressed data. */
	} Elf64_Chdr;

	typedef struct
	{
		Elf64_Word 	sh_name; 		/* Section name */
		sectionType sh_type; 		/* Section type */
		sectionFlag sh_flags; 		/* Section attributes */
		Elf64_Addr 	sh_addr; 		/* Virtual address in memory */
		Elf64_Off 	sh_offset; 		/* Offset in file */
		Elf64_Xword sh_size; 		/* Size of section */
		Elf64_Word 	sh_link; 		/* Link to other section */
		Elf64_Word 	sh_info; 		/* Miscellaneous information */
		Elf64_Xword sh_addralign; 	/* Address alignment boundary */
		Elf64_Xword sh_entsize; 	/* Size of entries, if section has table */
	} Elf64_Shdr;

	// Symbol table

	enum symbolBindings : unsigned char
	{
		STB_LOCAL = 0, 		/* Not visible outside the object file */
		STB_GLOBAL = 1, 	/* Global symbol, visible to all object files */
		STB_WEAK = 2, 		/* Global scope but with lower precedence than global symbols */
		STB_LOOS = 10, 		/* Environment-specific use */
		STB_HIOS = 12,
		STB_LOPROC = 13, 	/* Processor-specific use */
		STB_HIPROc = 15,
	};

	enum symbolType : unsigned char
	{
		STT_NOTYPE = 0, 	/* No type specified */
		STT_OBJECT = 1, 	/* Data object */
		STT_FUNC = 2, 		/* Function entry point */
		STT_SECTION = 3, 	/* Symbol associated with a section */
		STT_FILE = 4, 		/* Source file associated wth object file */
		STT_COMMON = 5, 	/* The symbol labels an uninitialized common block. */
		STT_TLS = 6,		/* The symbol specifies a Thread-Local Storage entity. */
		STT_LOOS = 10, 		/* Environment-specific use */
		STT_IFUNC = 10,
		STT_HIOS = 12,
		STT_LOPROC = 13, 	/* Processor-specific use */
		STT_HIPROC = 15,
	};

	enum symbolVisibility
	{
		STV_DEFAULT = 0,
		STV_INTERNAL = 1,
		STV_HIDDEN = 2,
		STV_PROTECTED = 3
	};

#define Elf64_Sym_Type(sym) (sym.st_info & 0x0F)
#define Elf64_Sym_Bind(sym) (sym.st_info >> 4)

	typedef struct
	{
		Elf64_Word 		st_name; 	/* Symbol name */
		unsigned char 	st_info; 	/* Type and Binding attributes */
		unsigned char 	st_other; 	/* Reserved */
		Elf64_Half 		st_shndx; 	/* Section table index */
		Elf64_Addr 		st_value; 	/* Symbol value */
		Elf64_Xword 	st_size; 	/* Size of object (e.g., common) */
	} Elf64_Sym;

	// Relocations

	typedef struct
	{
		Elf64_Addr 	r_offset; 	/* Address of reference */
		Elf64_Xword r_info; 	/* Symbol index and type of relocation */
	} Elf64_Rel;

	typedef struct
	{
		Elf64_Addr 		r_offset; 	/* Address of reference */
		Elf64_Xword 	r_info; 	/* Symbol index and type of relocation */
		Elf64_Sxword 	r_addend; 	/* Constant part of expression */
	} Elf64_Rela;

	// Program header table

	enum programType : Elf64_Word
	{
		PT_NULL = 0,					/* Unused entry */
		PT_LOAD = 1, 					/* Loadable segment */
		PT_DYNAMIC = 2, 				/* Dynamic linking table */
		PT_INTERP = 3, 					/* Program interpreter path name */
		PT_NOTE = 4, 					/* Note sections */
		PT_SHLIB = 5, 					/* Reserved */
		PT_PHDR = 6, 					/* Program header table */
		PT_TLS = 7,						/* The array element specifies the Thread-Local Storage template. */
		PT_LOOS = 0x60000000, 			/* Environment-specific use */
		PT_GNU_EH_FRAME = 0x6474E550,	/* GNU eh frame */
		PT_GNU_STACK = 0x6474E551,		/* GNU stack */
		PT_GNU_RELRO = 0x6474E552,		/* GNU relro */
		PT_GNU_PROPERTY = 0x6474E553,	/* GNU property */
		PT_HIOS = 0x6FFFFFFF,
		PT_LOPROC = 0x70000000, 		/* Processor-specific use */
		PT_HIPROC = 0x7FFFFFFF,
	};

	enum programFlag : Elf64_Word
	{
		PF_X = 0x1,					/* Execute permission */
		PF_W = 0x2,					/* Write permission */
		PF_R = 0x4,					/* Read permission */
		PF_MASKOS = 0x00FF0000, 	/* Environment-specific use */
		PF_MASKPROC = 0xFF000000, 	/* Processor-specific use */
	};

	typedef struct
	{
		programType	p_type; 	/* Type of segment */
		programFlag	p_flags; 	/* Segment attributes */
		Elf64_Off 	p_offset; 	/* Offset in file */
		Elf64_Addr 	p_vaddr; 	/* Virtual address in memory */
		Elf64_Addr 	p_paddr; 	/* Reserved */
		Elf64_Xword p_filesz; 	/* Size of segment in file */
		Elf64_Xword p_memsz; 	/* Size of segment in memory */
		Elf64_Xword p_align; 	/* Alignment of segment */
	} Elf64_Phdr;

	/*typedef struct {
		p_offset	File offset of the TLS initialization image
		p_vaddr	Virtual memory address of the TLS initialization image
		p_paddr	reserved
		p_filesz	Size of the TLS initialization image
		p_memsz	Total size of the TLS template
		p_flags	PF_R
		p_align	Alignment of the TLS template
	} Elf64_TLS_Entry;*/

	// Note sections

	typedef struct
	{
		Elf64_Xword n_namesz;	/* Name size for owner name */
		Elf64_Xword n_descsz;	/* Desc size */
		Elf64_Xword n_type;		/* Type (depends on owner name) */
		char n_name_desc[];		/* Name string (8 byte aligned) + desc string */
	} Elf64_Note_Hdr;

	// Dynamic table

	enum dynTag : Elf64_Sxword
	{
		DT_NULL = 0, 			/* ignored */ 	/* Marks the end of the dynamic array */
		DT_NEEDED = 1, 			/* d_val */ 	/* The string table offset of the name of a needed library. */
		DT_PLTRELSZ = 2, 		/* d_val */ 	/* Total size, in bytes, of the relocation entries associated with the procedure linkage table. */
		DT_PLTGOT = 3, 			/* d_ptr */ 	/* Contains an address associated with the linkage table. The specific meaning of this field is processor-dependent. */
		DT_HASH = 4, 			/* d_ptr */ 	/* Address of the symbol hash table, described below. */
		DT_STRTAB = 5, 			/* d_ptr */ 	/* Address of the dynamic string table. */
		DT_SYMTAB = 6, 			/* d_ptr */ 	/* Address of the dynamic symbol table. */
		DT_RELA = 7, 			/* d_ptr */ 	/* Address of a relocation table with Elf64_Rela entries. */
		DT_RELASZ = 8, 			/* d_val */ 	/* Total size, in bytes, of the DT_RELA relocation table. */
		DT_RELAENT = 9, 		/* d_val */ 	/* Size, in bytes, of each DT_RELA relocation entry. */
		DT_STRSZ = 10, 			/* d_val */ 	/* Total size, in bytes, of the string table. */
		DT_SYMENT = 11, 		/* d_val */ 	/* Size, in bytes, of each symbol table entry. */
		DT_INIT = 12, 			/* d_ptr */ 	/* Address of the initialization function. */
		DT_FINI = 13, 			/* d_ptr */ 	/* Address of the termination function. */
		DT_SONAME = 14, 		/* d_val */ 	/* The string table offset of the name of this shared object. */
		DT_RPATH = 15, 			/* d_val */ 	/* The string table offset of a shared library search path string. */
		DT_SYMBOLIC = 16, 		/* ignored */ 	/* The presence of this dynamic table entry modifies the symbol resolution algorithm for references within the library. Symbols,  defined within the library are used to resolve references before the dynamic linker searches the usual search path. */
		DT_REL = 17, 			/* d_ptr */ 	/* Address of a relocation table with Elf64_Rel entries. */
		DT_RELSZ = 18, 			/* d_val */ 	/* Total size, in bytes, of the DT_REL relocation table. */
		DT_RELENT = 19, 		/* d_val */ 	/* Size, in bytes, of each DT_REL relocation entry. */
		DT_PLTREL = 20, 		/* d_val */ 	/* Type of relocation entry used for the procedure linkage table. The d_val member contains either DT_REL or DT_RELA. */
		DT_DEBUG = 21, 			/* d_ptr */ 	/* Reserved for debugger use. */
		DT_TEXTREL = 22, 		/* ignored */ 	/* The presence of this dynamic table entry signals that the relocation table contains relocations for a non-writable segment. */
		DT_JMPREL = 23, 		/* d_ptr */ 	/* Address of the relocations associated with the procedure linkage table. */
		DT_BIND_NOW = 24, 		/* ignored */ 	/* The presence of this dynamic table entry signals that the dynamic loader should process all relocations for this object before, transferring control to the program. */
		DT_INIT_ARRAY = 25,		/* d_ptr */ 	/* Pointer to an array of pointers to initialization functions. */
		DT_FINI_ARRAY = 26, 	/* d_ptr */ 	/* Pointer to an array of pointers to termination functions. */
		DT_INIT_ARRAYSZ = 27, 	/* d_val */ 	/* Size, in bytes, of the array of initialization functions. */
		DT_FINI_ARRAYSZ = 28, 	/* d_val */ 	/* Size, in bytes, of the array of termination functions. */
		DT_RUNPATH = 29,		/* d_val */ 	/* String table offset of a null-terminated library search path string */
		DT_FLAGS = 30,			/* d_val */ 	/* Flag values specific to the object being loaded. Defined by DF_FLAGValues. */
		DT_ENCODING = 32,		/* */			/* */
		DT_PREINIT_ARRAY = 32,	/* d_ptr */ 	/* Address of the array of pointers to pre-initialization functions */
		DT_PREINIT_ARRAYSZ = 33,/* d_val */ 	/* Size in bytes of the array of pre-initialization functions pointed to by the DT_PREINIT_ARRAY entry. */
		DT_SYMTAB_SHNDX = 34,	/* d_ptr */ 	/* Address of the SHT_SYMTAB_SHNDX section associated with the dynamic symbol table referenced by the DT_SYMTAB element. */
		DT_LOOS = 0x60000000, 		/* ignored */ 	/* Defines a range of dynamic table tags that are reserved for environment-specific use. */
		DT_VALRNGLO = 0x6ffffd00,	/* ignored */	/* Defines a range of values using the d_un.d_val field of the dynamic structure.*/
		DT_CHECKSUM = 0x6ffffdf8, 	/* val */		/* A simple checksum of selected sections of the object. */
		DT_PLTPADSZ = 0x6ffffdf9, 	/* val */		/* */
		DT_MOVEENT = 0x6ffffdfa, 	/* val */		/* The size, in bytes, of the DT_MOVETAB move entries. */
		DT_MOVESZ = 0x6ffffdfb, 	/* val */		/* The total size, in bytes, of the DT_MOVETAB table. */
		DT_POSFLAG_1 = 0x6ffffdfd, 	/* val */		/* Various state flags which are applied to the DT_ element immediately following. */
		DT_SYMINSZ = 0x6ffffdfe, 	/* val */		/* The total size, in bytes, of the DT_SYMINFO table. */
		DT_SYMINENT = 0x6ffffdff, 	/* val */		/* The size, in bytes, of the DT_SYMINFO information entry. */
		DT_VALRNGHI = 0x6ffffdff,	/* ignored */	/* */
		DT_ADDRRNGLO = 0x6ffffe00,	/* ignored */	/* Defines a range of values using the d_un.d_ptr field of the dynamic structure.*/
		DT_GNU_HASH = 0x6ffffef5, 	/* ptr */		/* */
		DT_CONFIG = 0x6ffffefa, 	/* ptr */		/* The DT_STRTAB string table offset of a null-terminated string defining a configuration file. */
		DT_DEPAUDIT = 0x6ffffefb, 	/* ptr */		/* The DT_STRTAB string table offset of a null-terminated string defining one or more audit libraries. */
		DT_AUDIT = 0x6ffffefc, 		/* ptr */		/* The DT_STRTAB string table offset of a null-terminated string defining one or more audit libraries. */
		DT_PLTPAD = 0x6ffffefd, 	/* ptr */		/* */
		DT_MOVETAB = 0x6ffffefe, 	/* ptr */		/* The address of a move table. */
		DT_SYMINFO = 0x6ffffeff, 	/* ptr */		/* The address of the symbol information table. */
		DT_ADDRRNGHI = 0x6ffffeff,	/* ignored */	/* */
		DT_VERSYM = 0x6ffffff0,		/* val */		/* */
		DT_RELACOUNT = 0x6ffffff9,	/* val */		/* Indicates the RELATIVE relocation count, which is produced from the concatenation of all Elf32_Rela, or Elf64_Rela relocations. */
		DT_RELCOUNT = 0x6ffffffa, 	/* val */		/* Indicates the RELATIVE relocation count, which is produced from the concatenation of all Elf32_Rel relocations. */
		DT_FLAGS_1 = 0x6ffffffb, 	/* val */		/* Flag values specific to this object. */
		DT_VERDEF = 0x6ffffffc,		/* ptr */		/* The address of the version definition table. */
		DT_VERDEFNUM = 0x6ffffffd, 	/* val */		/* The number of entries in the DT_VERDEF table. */
		DT_VERNEED = 0x6ffffffe, 	/* ptr */		/* The address of the version dependency table. */
		DT_VERNEEDNUM = 0x6fffffff, /* val */		/* The number of entries in the DT_VERNEEDNUM table. */
		DT_HIOS = 0x6FFFFFFF,		/* ignored */	/* */
		DT_LOPROC = 0x70000000,		/* ignored */ 	/* Defines a range of dynamic table tags that are reserved for processor-specific use. */
		DT_HIPROC = 0x7FFFFFFF,
	};

	enum DF_FLAGValues
	{
		DF_ORIGIN = 0x1,
		DF_SYMBOLIC = 0x2,
		DF_TEXTREL = 0x4,
		DF_BIND_NOW = 0x8,
		DF_STATIC_TLS = 0x10
	};

	enum DF_FLAG_1Values
	{
		DF_1_NOW = 0x1, 			/*Perform complete relocation processing. */
		DF_1_GLOBAL = 0x2, 			/*Unused. */
		DF_1_GROUP = 0x4, 			/*Indicate object is a member of a group. */
		DF_1_NODELETE = 0x8, 		/*Object cannot be deleted from a process. */
		DF_1_LOADFLTR = 0x10, 		/*Ensure immediate loading of filtees. */
		DF_1_INITFIRST = 0x20, 		/*Objects' initialization occurs first. */
		DF_1_NOOPEN = 0x40, 		/*Object can not be used with dlopen(3C). */
		DF_1_ORIGIN = 0x80, 		/*$ORIGIN processing required. */
		DF_1_DIRECT = 0x100, 		/*Direct bindings enabled. */
		DF_1_INTERPOSE = 0x400, 	/*Object is an interposer. */
		DF_1_NODEFLIB = 0x800, 		/*Ignore the default library search path. */
		DF_1_NODUMP = 0x1000, 		/*Object cannot be dumped with dldump(3C). */
		DF_1_CONFALT = 0x2000, 		/*Object is a configuration alternative. */
		DF_1_ENDFILTEE = 0x4000, 	/*Filtee terminates filter's search. */
		DF_1_DISPRELDNE = 0x8000, 	/*Displacement relocation has been carried out. */
		DF_1_DISPRELPND = 0x10000, 	/*Displacement relocation pending. */
		DF_1_NODIRECT = 0x20000, 	/*Object contains non - direct bindings. */
		DF_1_IGNMULDEF = 0x40000, 	/*Internal use. */
		DF_1_NOKSYMS = 0x80000, 	/*Internal use. */
		DF_1_NOHDR = 0x100000, 		/*Internal use. */
		DF_1_EDITED = 0x200000, 	/*Object has been modified since originally built. */
		DF_1_NORELOC = 0x400000, 	/*Internal use. */
		DF_1_SYMINTPOSE = 0x800000, /*Individual symbol interposers exist. */
		DF_1_GLOBAUDIT = 0x1000000, /*Establish global auditing. */
		DF_1_SINGLETON = 0x2000000, /*Singleton symbols exist. */
		DF_1_PIE = 0x8000000		/* */
	};

	typedef struct
	{
		Elf64_Xword d_tag;
		union
		{
			Elf64_Xword d_val;
			Elf64_Addr d_ptr;
		} d_un;
	} Elf64_Dyn;

	// Hash table

	typedef struct
	{
		Elf64_Word h_nbucket;
		Elf64_Word h_nchain;
		char h_bucket_chain;
	} Elf64_Hash_Hdr;

	size_t elf64_hash(const unsigned char* name);

#ifdef __cplusplus
}
#endif
