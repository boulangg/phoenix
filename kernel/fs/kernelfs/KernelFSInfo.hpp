#pragma once

#include "fs/vfs/BaseFS.hpp"

class KernelDentry;
class KernelFile;
class KernelInode;
class KernelSuperBlock;
class KernelAddressSpace;

typedef BaseFSInfo<KernelSuperBlock, KernelInode, KernelDentry, KernelFile, KernelAddressSpace> KernelFSInfo;

#define TYPE_DIR 0x1
#define TYPE_FILE 0x2
