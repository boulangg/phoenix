/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "KernelInode.hpp"

#include "KernelAddressSpace.hpp"
#include "KernelFile.hpp"
#include "KernelSuperBlock.hpp"

KernelInode::KernelInode(KernelSuperBlock* sb) :
    BaseInode(sb, 0, user_apps_symbol_table.nb_user_apps), type(TYPE_FILE), sb(sb), app()
{
    mapping = new KernelAddressSpace(this);
}

KernelInode::KernelInode(KernelSuperBlock* sb, std::uint64_t ino, struct apps_desc app) :
    BaseInode(sb, ino, app.apps_end - app.apps_start), type(TYPE_FILE), sb(sb), app(app)
{
    mapping = new KernelAddressSpace(this);
}

KernelInode::~KernelInode() {}

// inode_operations
Dentry* KernelInode::create(Dentry*, std::string)
{
    return nullptr;
}

Dentry* KernelInode::lookup(Dentry* parent, std::string name)
{
    if (type == TYPE_DIR) {
        KernelInode* inode = sb->getFile(name);
        if (inode != nullptr) {
            return new KernelDentry(parent, inode, name);
        }
    }
    return nullptr;
}

// file_operation
File* KernelInode::open_internal()
{
    if (type == TYPE_DIR) {
        return nullptr;
    } else {
        return new KernelFile(this);
    }
}
