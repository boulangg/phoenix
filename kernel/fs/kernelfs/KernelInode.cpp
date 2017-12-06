#include "KernelInode.hpp"

#include "KernelSuperBlock.hpp"
#include "KernelFile.hpp"

KernelInode::KernelInode(KernelSuperBlock* sb, std::uint64_t ino) : BaseInode(sb, ino),
		type(TYPE_FILE), sb(sb), app() {
}

KernelInode::KernelInode(KernelSuperBlock* sb, std::uint64_t ino, struct apps_desc app) :
	BaseInode(sb, ino), type(TYPE_FILE), sb(sb), app(app) {
}


KernelInode::~KernelInode() {

}

// inode_operations
Dentry* KernelInode::create(Dentry*, std::string) {
	return nullptr;
}

Dentry * KernelInode::lookup(Dentry* parent, std::string name) {
	if (type == TYPE_DIR) {
		KernelInode* inode = sb->getFile(name);
		if (inode != nullptr) {
			return new KernelDentry(parent, inode, name);
		}
	}
	return nullptr;
}

// file_operation
KernelInode::file_t* KernelInode::doOpen() {
	if (type == TYPE_DIR) {
		return nullptr;
	} else {
		return new KernelFile(this);
	}
}
