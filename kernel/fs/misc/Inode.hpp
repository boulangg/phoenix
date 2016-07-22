#ifndef __INODE_HPP__
#define __INODE_HPP__

#include "Superblock.hpp"

class Inode {
public:
	Inode(Superblock* sb);
	~Inode();

	void write(int wait);
	void dirty();
};

#endif // __INODE_HPP__
