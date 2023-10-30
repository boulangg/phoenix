/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #include "VFS.h"

#include "Kernel.h"
#include <fs/ramfs/RamSuperBlock.h>

namespace kernel::fs {

std::unordered_map<std::pair<DEntry*, std::string>, DEntry*> VFS::_dentryCache;

FsHandler VFS::getInitFsHandler()
{
    fs::ramfs::RamSuperBlock* sb = new fs::ramfs::RamSuperBlock();
    fs::Inode* root = sb->getRootInode();
    fs::FsHandler handler;
    handler.root.dentry = new fs::DEntry(root);
    handler.root.mount = new fs::vfsmount();
    handler.root.mount->root = handler.root.dentry;
    handler.root.mount->sb = sb;
    handler.root.mount->parent = nullptr;
    handler.root.mount->mountpoint = nullptr;

    return handler;
}

FilePath VFS::lookup(FsHandler handler, const std::string& pathName)
{
    FilePath currentPath;
    if (pathName.starts_with('/')) {
        currentPath = handler.root;
    } else {
        currentPath = handler.pwd;
    }

    std::size_t i = 0;
    // Skip leading /
    while (pathName[i] == '/') {
        i++;
    }

    // If empty, return current DEntry
    if (i == pathName.size()) {
        return currentPath;
    }

    do {
        // Compute current pathname component
        auto pos = pathName.find('/', i);
        if (pos == std::string::npos) {
            // Last component of the path
            break;
        }

        std::string currentPathNameComponent = pathName.substr(i, pos - i);

        Inode* inode = currentPath.dentry->getInode();
        // TODO: Check permission

        // Skip leading /
        while (pathName[pos] == '/') {
            pos++;
        }

        FilePath nextPath = lookupPathComponent(currentPath, currentPathNameComponent);

        // TODO Check if nextDEntry is a directory (returns an error otherwise)

        currentPath = nextPath;

        i = pos;
    } while (i < pathName.size());

    std::string lastPathNameComponent = pathName.substr(i);

    FilePath nextPath = lookupPathComponent(currentPath, lastPathNameComponent);

    // TODO Check if nextDEntry.inode == NULL (file doesn't exist)

    // TODO If LOOKUP_DIRECTORY flag, check if nextDEntry is directory (not a dir)

    return nextPath;
}
FilePath VFS::parentPathComponent(FilePath currentPath)
{
    FilePath nextPath = currentPath;
    // Check if parent is a mount point
    while (nextPath.mount->root == nextPath.dentry && nextPath.mount->mountpoint != nullptr) {
        nextPath.mount = nextPath.mount->parent;
        nextPath.dentry = nextPath.mount->mountpoint;
    }
    nextPath.dentry = currentPath.dentry->getParent();

    return nextPath;
}

FilePath VFS::lookupPathComponent(FilePath currentPath, std::string currentPathNameComponent)
{
    // Check "."
    if (currentPathNameComponent == ".") {
        return currentPath;
    }

    // Check ".."
    if (currentPathNameComponent == "..") {
        return parentPathComponent(currentPath);
    }

    FilePath nextPath = currentPath;
    auto key = std::make_pair(currentPath.dentry, currentPathNameComponent);
    if (_dentryCache.contains(key)) {
        nextPath.dentry = _dentryCache.at(key);
    } else {
        Inode* nextInode = currentPath.dentry->getInode()->lookup(currentPathNameComponent);
        // TODO error if nextInode is null ?
        nextPath.dentry = new DEntry(currentPath.dentry, nextInode, currentPathNameComponent);
        _dentryCache[key] = nextPath.dentry;
    }

    // Check if nextDEntry is a mount point
    while (nextPath.mount->children.contains(nextPath.dentry)) {
        nextPath.mount = nextPath.mount->children.at(nextPath.dentry);
        nextPath.dentry = nextPath.mount->root;
    }

    // TODO Check if nextDEntry is a symbolik link

    return nextPath;
}

void VFS::mount(FilePath mountPoint, SuperBlock* sb)
{
    vfsmount* newMount = new vfsmount();
    newMount->sb = sb;
    newMount->root = new DEntry(sb->getRootInode());
    newMount->mountpoint = mountPoint.dentry;
    newMount->parent = mountPoint.mount;
    mountPoint.mount->children[mountPoint.dentry] = newMount;
}

}
