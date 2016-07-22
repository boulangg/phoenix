#ifndef __DIRECTORY_HPP__
#define __DIRECTORY_HPP__

#include <cstddef>
#include "Inode.hpp"

class Directory : public Inode {
public:
	Directory(char* dirname);

	Directory* openDir(char* dirName);
	File* openFile(char* filename);
private:
	char* dirname;
};


#endif // __DIRECTORY_HPP__
