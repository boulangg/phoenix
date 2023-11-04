#pragma once

#include "fs/vfs/BaseFS.hpp"

#include "KernelInode.hpp"
#include "KernelDentry.hpp"
#include <fs/vfs/VirtualFileSystem.hpp>

class KernelSuperBlock : public BaseSuperBlock<KernelFSInfo>
{

public:
	KernelSuperBlock(FileSystemType* type, const std::string& source) : BaseSuperBlock(type)
	{
		std::vector<std::string> pathnameVector = VirtualFileSystem::parsePathname(source);
		Dentry* src = DentryCache::findDentry(VirtualFileSystem::root, pathnameVector, 0);
		(void)src;

		//char* apps = apps;
		inoderoot = new KernelInode(this);
		inoderoot->type = TYPE_DIR;
		//_files.push_back(inoderoot);
		root = new KernelDentry(inoderoot);
		for (uint64_t i = 0; i < user_apps_symbol_table.nb_user_apps; ++i) {
			struct apps_desc app = user_apps_symbol_table.apps[i];
			KernelInode* appInode = new KernelInode(this, i + 1, app);
			_files.push_back(appInode);
		}
	}

	KernelInode* getFile(std::string name)
	{
		for (auto file : _files) {
			if (name.compare(*(file->app.apps_name)) == 0) {
				return file;
			}
		}
		return nullptr;
	}

	virtual Inode* allocInode()
	{
		return nullptr;
	}

	std::list< KernelInode* > _files;
	KernelInode* inoderoot;
};
