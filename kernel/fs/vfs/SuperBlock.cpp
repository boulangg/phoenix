#include "SuperBlock.hpp"

SuperBlock::SuperBlock(FileSystemType* type, Dentry* root) :
	type(type), root(root), dev(nullptr), blockSize(1024) {

}

SuperBlock::SuperBlock(FileSystemType* type) :
	type(type), root(nullptr), dev(nullptr), blockSize(1024) {

}

SuperBlock::~SuperBlock() {

}

