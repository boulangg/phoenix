/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "DentryCache.hpp"

#include "Inode.hpp"

Dentry* DentryCache::findDentry(Dentry* parent, std::vector<std::string> pathname, size_t index)
{
    if (parent->mount != nullptr) {
        return findDentry(parent->mount, pathname, index);
    }
    if (index == pathname.size()) {
        return parent;
    }
    for (auto child : parent->children) {
        if (child->name.compare(pathname[index]) == 0) {
            return findDentry(child, pathname, index + 1);
        }
    }

    Dentry* child = parent->inode->lookup(parent, pathname[index]);
    if (child != nullptr) {
        parent->children.push_back(child);
        return findDentry(child, pathname, index + 1);
    }
    return nullptr;
}
