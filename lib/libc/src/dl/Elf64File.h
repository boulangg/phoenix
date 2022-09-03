#include "elf64.h"

#include <cstdio>
#include <cstdint>
#include <string>

namespace dl {

class Elf64File
{
public:
	Elf64File(const std::string& filePath);
	~Elf64File();

	void load(Elf64_Addr startAddr, int mode);
	void unload();

	Elf64_Addr getSymbol(const std::string& symbolName);

	void incrementCount();
	void decrementCount();
	std::size_t getCount();

	const std::string& getFileName();

private:
	std::string _filename;
	FILE* _fileDescriptor;
	std::size_t _count;
	Elf64_Ehdr _Elf64Header;
};
}
