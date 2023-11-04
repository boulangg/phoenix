/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #include "Ext2FSInfo.hpp"

#include <core/Debug.hpp>

check_size<sizeof(ext2_superblock_t), 1024> test_size_superblock;
check_size<sizeof(ext2_block_group_desc_t), 32> test_size_block_group_desc;
check_size<sizeof(ext2_inode_data_t), 128> test_size_inode;
check_size<sizeof(ext2_directory_entry_header_t), 8> test_size_directory_entry;
