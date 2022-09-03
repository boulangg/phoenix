#include "Elf64File.h"

#include <sys/mman.h>

using namespace dl;

Elf64File::Elf64File(const std::string& filename) :
	_filename(filename), _fileDescriptor(nullptr), _count(1)
{

}

Elf64File::~Elf64File()
{
	if (_fileDescriptor != nullptr) {
		unload();
	}
}

void Elf64File::load(Elf64_Addr startAddr, int mode)
{
	//	void* start = (void*)0x10EA52000;
	//	size_t pagesize = getpagesize();
	//	struct stat buf;
	//	stat(file, &buf);
	//	int fd = fopen("plugin.so", 0, 0);
	//	mmap(start+(0*pagesize), 6*pagesize,               PROT_READ|PROT_EXEC,  MAP_FIXED|MAP_PRIVATE, fd, 0*pagesize);
	//	mmap(start+(6*pagesize), 1*pagesize,               PROT_READ|PROT_WRITE, MAP_FIXED|MAP_PRIVATE, fd, 1*pagesize);
	//	mmap(start+(7*pagesize), stat.off_t - 7*pagesize,  PROT_READ,            MAP_FIXED|MAP_PRIVATE, fd, 2*pagesize);

	(void)startAddr;
	(void)mode;
	_fileDescriptor = fopen(_filename.c_str(), "rb");

	fread((void*)&_Elf64Header, sizeof(Elf64_Ehdr), 1, _fileDescriptor);

	printf("ELF Header:\n");
	printf("  Magic:");
	for (size_t i = 0; i < EI_NIDENT; i++) {
		printf(" %uc", _Elf64Header.e_ident[i]);
	}
	printf("\n");
	printf("  Class: %i\n", _Elf64Header.e_type);
	printf("  Machine: %i\n", _Elf64Header.e_machine);
	printf("  Version: %u\n", _Elf64Header.e_version);
	printf("  Entry: %lu\n", _Elf64Header.e_entry);
	printf("  program header: %lu\n", _Elf64Header.e_phoff);
}

Elf64_Addr Elf64File::getSymbol(const std::string& symbolName)
{
	(void)symbolName;
	return 0;
}

void Elf64File::unload()
{
// close(fd);
// munmap(start+(0*pagesize), 4096);
// munmap(start+(6*pagesize), 4096);
// munmap(start+(7*pagesize), 4096);


	int res = fclose(_fileDescriptor);
	(void)res;
}

void Elf64File::incrementCount()
{
	_count++;
}

void Elf64File::decrementCount()
{
	if (_count > 0) {
		_count--;
	}
}

std::size_t Elf64File::getCount()
{
	return _count;
}

const std::string& Elf64File::getFileName()
{
	return _filename;
}
