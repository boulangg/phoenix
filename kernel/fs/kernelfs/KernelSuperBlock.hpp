#pragma once

#include "fs/vfs/BaseFS.hpp"

#include "KernelInode.hpp"
#include "KernelDentry.hpp"

class KernelSuperBlock : public SuperBlock {

public:
	KernelSuperBlock(FileSystemType* type, Dentry* src) : SuperBlock(type) {
		(void)src;
		//char* apps = apps;
		inoderoot = new KernelInode(this, 0);
		inoderoot->type = TYPE_DIR;
		//_files.push_back(inoderoot);
		root = new KernelDentry(inoderoot);
		for (uint64_t i = 0; i < user_apps_symbol_table.nb_user_apps; ++i) {
			struct apps_desc app = user_apps_symbol_table.apps[i];
			KernelInode* appInode = new KernelInode(this, i+1, app);
			_files.push_back(appInode);
		}
	}

	KernelInode* getFile(std::string name) {
		for (auto file : _files) {
			if (name.compare(*(file->app.apps_name)) == 0) {
				return file;
			}
		}
		return nullptr;
	}

	virtual Inode* allocInode() {
		return nullptr;
	}

	std::list< KernelInode* > _files;
	KernelInode* inoderoot;
};
