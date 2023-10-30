/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <list>
#include <string>
#include <unordered_map>
#include <utility>

#include "DEntry.h"
#include "SuperBlock.h"

namespace kernel::fs {

// struct vfsmount;
//
// struct fs_namespace
//{
//    int refCount; // nb of process sharing this table
//    vfsmount* root;
//    std::list<vfsmount*> list;
//};

struct vfsmount
{
    // int refCount; // Usage counter (increased to forbid filesystem unmounting).
    // int flags;
    vfsmount* parent;
    DEntry* mountpoint;
    DEntry* root;
    SuperBlock* sb;
    std::unordered_map<DEntry*, vfsmount*> children;
    // char* devname;
    // fs_namespace* _namespace;
};

struct FilePath
{
    DEntry* dentry;
    vfsmount* mount;
};

struct FsHandler
{
    // int refCount; // nb of process sharing this table
    // int umask;    // Bit mask used when opening the file to set the file permissions
    FilePath root;
    FilePath pwd;
};
}

namespace kernel::fs {

class VFS
{
public:
    static FsHandler getInitFsHandler();
    static FilePath lookup(FsHandler handler, const std::string& pathName);
    static FilePath parentPathComponent(FilePath currentPath);
    static FilePath lookupPathComponent(FilePath currentPath, std::string currentPathNameComponent);
    static void mount(FilePath mountPoint, SuperBlock* sb);

private:
    static std::unordered_map<std::pair<DEntry*, std::string>, DEntry*> _dentryCache;
};

}
