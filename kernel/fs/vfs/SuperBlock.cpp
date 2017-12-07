#include "SuperBlock.hpp"

SuperBlock::SuperBlock(FileSystemType* type, Dentry* root) :
	dev(nullptr), blockSize(1024), type(type), root(root)  {

}

SuperBlock::SuperBlock(FileSystemType* type) :
		dev(nullptr), blockSize(1024), type(type), root(nullptr) {

}

SuperBlock::~SuperBlock() {

}

