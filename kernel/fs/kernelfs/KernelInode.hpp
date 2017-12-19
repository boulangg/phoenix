#pragma once

#include "KernelFSInfo.hpp"

class KernelSuperBlock;

#include <boot/processor_struct.hpp>

class KernelInode : public BaseInode<KernelFSInfo> {
public:
	virtual ~KernelInode();

	// inode_operations
	virtual Dentry* create(Dentry*, std::string);

	virtual Dentry * lookup(Dentry* parent, std::string name);

	// file_operation


	KernelInode(KernelSuperBlock* sb, std::uint64_t ino);

	KernelInode(KernelSuperBlock* sb, std::uint64_t ino, struct apps_desc app);

private:
	virtual File* open_internal() override;

//protected:
public:
	int type;
	KernelSuperBlock* sb;
	struct apps_desc app;
};

