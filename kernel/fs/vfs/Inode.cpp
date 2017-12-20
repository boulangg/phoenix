#include "Inode.hpp"

#include "SuperBlock.hpp"

Inode::Inode(SuperBlock* sb, std::uint64_t ino, size_t size) :
		sb(sb), ino(ino), size(size), mapping(nullptr), dentries()
{

}

Inode::~Inode() {
}

Dentry* Inode::lookup(Dentry* parent, std::string name) {
	(void)parent; (void)name;
	return nullptr;
}

File* Inode::open() {
	return open_internal();
}
