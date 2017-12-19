#include "Inode.hpp"

#include "SuperBlock.hpp"

Inode::Inode(SuperBlock* sb, std::uint64_t ino) :
		sb(sb), ino(ino), size(0), mapping(nullptr), dentries()
{
	size = 0;
	mapping = nullptr;

	//mapping = new AddressSpace();
	//size = 12;
}

Inode::~Inode() {
}

/*Dentry* Inode::create(Dentry* parent, std::string name) {
	// TODO check mode?
	return new Dentry(parent, this, name);
}*/

Dentry* Inode::lookup(Dentry* parent, std::string name) {
	(void)parent; (void)name;
	return nullptr;
}

File* Inode::open() {
	return open_internal();
}
