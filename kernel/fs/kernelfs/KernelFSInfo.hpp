#pragma once

#include "fs/vfs/BaseFS.hpp"

class KernelDentry;
class KernelFile;
class KernelInode;
class KernelSuperBlock;

typedef BaseFSInfo<KernelDentry, KernelFile, KernelInode, KernelSuperBlock> KernelFSInfo;

#define TYPE_DIR 0x1
#define TYPE_FILE 0x2


