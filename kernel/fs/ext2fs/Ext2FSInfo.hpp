#pragma once

#include <fs/vfs/BaseFS.hpp>

#include <cstdint>

class Ext2SuperBlock;
class Ext2Inode;
class Ext2Dentry;
class Ext2File;
class Ext2AddressSpace;

typedef BaseFSInfo<Ext2SuperBlock, Ext2Inode, Ext2Dentry, Ext2File, Ext2AddressSpace> Ext2FSInfo;

#define EXT2_SIGNATURE 0xEF53 /**< ext2 signature*/
#define ROOT_INODE 2 /**< root inode*/

typedef struct {
    std::uint32_t nb_inodes;
    std::uint32_t nb_blocks;
    std::uint32_t nb_blocks_su;
    std::uint32_t nb_blocks_unalloc;
    std::uint32_t nb_inodes_unalloc;
    std::uint32_t superblock_num;
    std::uint32_t log_block_size;
    std::uint32_t log_fragment_size;
    std::uint32_t block_group_blocks;
    std::uint32_t block_group_fragments;
    std::uint32_t block_group_inodes;
    std::uint32_t last_mount_time;
    std::uint32_t last_written_time;
    std::uint16_t nb_mount;
    std::uint16_t nb_mount_left;
    std::uint16_t signature;
    std::uint16_t state;
    std::uint16_t error;
    std::uint16_t version_minor;
    std::uint32_t check_time;
    std::uint32_t next_check_time;
    std::uint32_t os_source_id;
    std::uint32_t version_major;
    std::uint16_t reserved_user_id;
    std::uint16_t reserved_group_id;
    std::uint32_t first_non_reserved_inode;
    std::uint16_t size_of_inode_struct;
    std::uint16_t block_group_backup;
    std::uint32_t optional_features_flags;
    std::uint32_t required_features_flags;
    std::uint32_t read_only_features_flags;
    std::uint32_t file_system_ID[4];
    char volume_name[16];
    char path_volume[64];
    std::uint32_t compression_algorithm;
    char nb_blocks_alloc_files;
    char nb_blocks_alloc_dir;
    std::uint16_t unused_1;
    std::uint32_t journal_ID[4];
    std::uint32_t journa_inode;
    std::uint32_t journal_device;
    std::uint32_t head_orphan_inode_list;
    std::uint8_t unused_2[787];
} ext2_superblock_t;

typedef enum {
    CLEAN = 1,
    ERRORS = 2
} ext2_file_system_state_t;

typedef enum {
    IGNORE_ERRO = 1,
    REMOUNT_AS_READ_ONLY = 2,
    KERNEL_PANIC = 3
} ext2_error_handling_method_;

typedef enum {
    LINUX = 0,
    GNU_HURD = 1,
    MASIX = 2,
    FREEBSD = 3,
    OTHER = 4
} ext2_creator_OS_system_ID_t;

typedef enum {
    PREALLOCATE_BLOCKS = 0x01,
    AFS_SERVER = 0x02,
    JOURNAL = 0x04,
    EXTENDED_INODES = 0x08,
    AUTO_RESIZE = 0x10,
    HASH_INDEX_DIR = 0x20
} ext2_optional_features_flags_t;

typedef enum {
    COMPRESSION = 0x01,
    DIR_HAS_TYPE_FIELD = 0x02,
    JOURNAL_REPLAY_NEEDED = 0x04,
    USE_JOURNAL_DEVICE = 0x08
} ext2_required_feature_flags_t;

typedef enum {
    SPARSE_BLOCKS = 0x01,
    FILE_SIZE_64_BIT = 0x02,
    DIR_BINARY_TREE = 0x04
} ext2_read_only_feature_flags_t;

typedef struct {
    std::uint32_t block_usage_bitmap_addr;
    std::uint32_t inode_usage_bitmap_addr;
    std::uint32_t start_inode_table;
    std::uint16_t nb_unallocated_blocks;
    std::uint16_t nb_unallocated_inodes;
    std::uint16_t nb_dir;
    char unused[14];
} ext2_block_group_desc_t;

typedef struct {
    std::uint16_t type_n_perm;
    std::uint16_t user_ID;
    std::uint32_t file_size_low;
    std::uint32_t last_access_time;
    std::uint32_t creation_time;
    std::uint32_t last_modif_time;
    std::uint32_t deletion_time;
    std::uint16_t group_ID;
    std::uint16_t nb_hard_link;
    std::uint32_t nb_disk_sectors;
    std::uint32_t flags;
    std::uint32_t OS_specific_value_1;
    std::uint32_t direct_block_addr[12];
    std::uint32_t simply_indirect_block_ptn;
    std::uint32_t doubly_indirect_block_ptn;
    std::uint32_t triply_indirect_block_ptn;
    std::uint32_t generation_num;
    std::uint32_t file_ACL;
    std::uint32_t file_size_high_or_dir_ACL;
    std::uint32_t fragment_block_addr;
    std::uint32_t OS_specific_value_2[3];
} ext2_inode_data_t;

typedef enum {
    // TYPE
    FIFO = 0x1000,
    CHARACTER_DEVICE = 0x2000,
    DIRECTORY = 0x4000,
    BLOCK_DEVICE = 0x6000,
    REGULAR_FILE = 0x8000,
    SYMBOLIC_LINK = 0xA000,
    UNIX_SOCKET = 0xC000,

    // PERM
    OTHER_E = 0x001,
    OTHER_W = 0x002,
    OTHER_R = 0x004,
    GROUP_E = 0x008,
    GROUP_W = 0x010,
    GROUP_R = 0x020,
    USER_E = 0x040,
    USER_W = 0x080,
    USER_R = 0x100,
    STICKY_BIT = 0x200,
    SET_GROUP_ID = 0x400,
    SET_USER_ID = 0x800
} ext2_type_n_perm_t;

typedef enum {
    SECURE_DELETION = 0x00000001,
    KEEP_COPY_DELETED = 0x00000002,
    FILE_COMPRESSION = 0x00000004,
    SYNCHRONUS_UPDATES = 0x00000008,
    IMMUTABLE_FILE = 0x00000010,
    APPEND_ONLY = 0x00000020,
    FILE_NOT_IN_DUMP = 0x00000040,
    DONT_UPDATE_ACCESS_TIME = 0x00000080,
    //RESERVED SPACE
    HASH_INDEXED_DIR = 0x00010000,
    AFS_DIRECTORY = 0x00020000,
    JOURNAL_FILE_DATA = 0x00040000
} ext2_inode_flag_t;

typedef struct {
    std::uint32_t inode;
    std::uint16_t entry_size;
    std::uint8_t name_length;
    std::uint8_t type_indicator;
} ext2_directory_entry_header_t;

typedef enum {
    UNKNOWN_TYPE = 0,
    REGULAR_FILE_ENTRY = 1,
    DIRECTORY_ENTRY = 2,
    CHAR_DEVICE = 3,
    BLOCK_DEVICE_ENTRY = 4,
    FIFO_ENTRY = 5,
    SOCKET = 6,
    SYMBOLIC_LINK_ENTRY = 7
} ext2_entry_type_t;
