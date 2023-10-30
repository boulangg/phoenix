/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include <cstddef>
#include <cstdint>

namespace kernel::vfs {

struct BlockDevice
{
    dev_t deviceId;
    char[] name;
};

// Stores information concerning a mounted filesystem. For disk-based filesystems, this object usually corresponds to a
// filesystem control block stored on disk.
struct SuperBlock
{
    // dev_t devId;
    std::uint32_t blockSize;
    // std::uint32_t blockSizeInBits;
    FileSystemType* type;
    std::uint32_t mountFlags;
    std::uint32_t fsMagicNumber;
    DEntry* root;
    int refCount;
    List<Inode*> inodes;
    List<Indoe*> dirtyInodes;
    List<Inode*> ioInodes;
    List<Files*> files;
    BlockDevice* dev;
    // char[] devName;

    Inode* alloc_inode();
    void destroy_inode(Inode*);
    void read_inode(Inode*);
    void dirty_Inode(Inode*);
    void write_inode(Inode*, std::int flag); // flags in whether the op should be synchronous or not.
    void put_inote(Inode*);    // Invoked when the inode is released—its reference counter is decreased—to perform
                               // filesystem - specific operations.
    void drop_inode(Inode*);   // Invoked when the inode is about to be destroyed—that is, when the last user releases
                               // the inode;
    void delete_inode(Inode*); // Delete VFS Inode in memorya and on disk
    void clear_inode(Inode*);  // Inovked when Inode is being destroyed

    void put_super(); // Unmount (release SuperBlock struct from memory)
    void write_super();
    void sync_fs(); // For journaling system
};

// Stores information about the linking of a directory entry (that is, a particular name of the file) with the
// corresponding file. Each disk-based filesystem stores this information in its own particular way on disk.
struct Dentry
{
    int refCount;
    uint32_t flags; // Dentry cache flags
    Inode* inode;
    Dentry* parent;
    qstr name; // Filename
    list<Dentry*> subdirs;
    // SuperBlock* sb;

    uint64_t hash(Dentry*, name); // Return hash
    compare(dir, name1, name2);   // Compares two filenames; name1 should belong to the directory referenced by dir.
    remove();                     // Called when the last reference to a dentry object is deleted (d_count becomes 0).
    release();                    // Called when a dentry object is going to be freed (released to the slab allocator).
    diput();                      // Called when a dentry object becomes “negative”—that is, it loses its inode.
};

// Stores general information about a specific file.For disk - based filesystems, this object usually corresponds to a
// file control block stored on disk.Each inode object is associated with an inode number, which uniquely identifies the
// file within the filesystem.
struct Inode
{
    list<DEntry*> dentries;
    std::uint32_t ino;
    int refCount;
    mode_t mode;    // File Type and acces right;
    uint32_t nlink; // Number of hard link
    // dev_t i_rdev; // Real device identifier
    std::size_t size;      // File size
    timespec lastAccessed; // File access
    timespec lastWrite;    // File write
    timespec lastChanged;  // Inode change
    // std::uint32_t blockSize;
    // std::uint32_t blockSizeInBits;
    uint32_t inodeVersion;
    uint32_t nbBlocks;
    uint16_t lastBlockSize;
    SuperBlock* sb;
    FileAddressSpace* addrSpace;
    // BlockDevice* bdev;
    // CharacterDevice* cdev;
    uint32_t dirtied_when;
    // std::uint32_t mountFlags;
    int writeCount;

    Inode* create(dir, entry, mode, nameidata);
    Inode* lookup(dir, entry, nameidata);
    link(old_dentry, dir, new_dentry); // Hard link
    unlink(dir, dentry);               // Remove hard link
    symlink(dir, dentry, synname);     // Symbolic link
    mkdir(dir, dentry, mode);
    rmdir(dir, dentry, mode);
    mknod(dir, dentry, mode, rdev);
    rename(old_dir, old_dentry, new_dir, new_entry);
    read_link(dentry, buffer, buflen);  // Copy filepath of the symlink to buffer
    follow_link(inode, nameidata);      // Translates a symbolic link specified by an inode object; if
                                        // the symbolic link is a relative pathname, the lookup operation
                                        // starts from the directory specified in the second parameter.
    put_link(dentry, nameidata);        // Releases all temporary data structures allocated by the follow_link method to
                                        // translate a symbolic link.
    truncate(inode);                    // Reduce file size
    permission(inode, mask, nameidata); // Check permission for the file
};

struct read_descriptor_t
{
    std::size_t written; // How many bytes have been copied into the User Mode buffer
    std::size_t count;   // How many bytes are yet to be transferred
    char* buf;           // Current position in the User Mode buffer
    int error;           // Error code of the read operation (0 for no error)
};

// Stores information about the interaction between an open file and a process. This information exists only in kernel
// memory during the period when a process has the file open.
struct File
{
    Dentry* dentry;
    vfsmount* vfsmont;
    int refCount;
    uint32_t flags; // Specified when opening the file
    mode_t mode;    // Process access mode
    loff_t offset;  // Current file offset
    uint32_t fileVersion;
    FileAddressSpace* addrSpace;

    llseek(file, offset, origin);
    std::size_t read(char* buf, std::size_t count)
    {
        Inode* inode = addrSpace->inode;
        std::size_t max = min(offset + count, inode->size);
        std::size_t start = offset;
        while (start < max) {
            std::size_t end = min(alignUp(start + 1, PAGE_SIZE), max);
            std::size_t indexInFile = start / PAGE_SIZE;

            FilePageDesc* page = addrSpace->get_or_alloc_page();
            if (!page.uptodate) {
                addrSpace->readPage(page);
            }
            std::size_t startInPage = start % PAGE_SIZE;
            std::size_t sizeInPage = end - start;
            std::memcpy(buf, (char*)(page.getKernelPageAddr() + startInPage), sizeInPage);
        }
    }

    void __generic_file_aio_read()
    {
        for (kiocb& req : requests) {
            read_descriptor_t desc(req);
            do_generic_file_read(desc, &req.ki_pos);
            req.ki_user_data = desc.written;
        }
    }

    void do_generic_file_read(read_descriptor_t* desc, std::size_t* ppos)
    {
        Inode* inode = addressSpace->inode;
        std::size_t index = (*ppos) / PAGE_SIZE;
        std::size_t offset = (*ppos) % PAGE_SIZE;

        while (desc.count > 0) {
            if (index * PAGE_SIZE + offset > inode->size) {
                break;
            }
            FilePageDesc* pagedesc = addressSpace.find_get_page(index); // increase page ref count
            if (page == NULL) {
                Page* page = MemoryAllocator::allocPage();
                pagedesc = new FilePageDesc(page, addressSpace, index);
                addressSpace.add_to_page_cache(page); // => this method lock_page()
                = > 4j
            } else {
                if (page.uptodate) {
                    = > 4m
                } else {
                    lock_page();
                    if (page.mapping == NULL) {
                        unlock_page();
                        page.decreaseRefCount();
                        continue;
                    }
                    if (page.uptodate) {
                        unlock_page();
                        = > 4m
                    }
                    = > 4j
                }
            }
            // 4j
            addressSpace.readPage(index); // => this will call unlock_page once the page is read
            if (!pagedesc.uptodate) {
                lock_page();
            }

            // File has been truncated by another process
            if (index * PAGE_SIZE > inode->size) {
                pagedesc.decreaseRefCount();
                break;
            }

            // 4m
            std::size_t nr = std::min(PAGE_SIZE, desc->count); // => TODO put the correct value here
            // page.mark_page_accessed(); // Page is in used and shouldn't be swapped
            file_read_actor();
            index = index + nr / PAGE_SIZE;
            offset = offset + nr % PAGE_SIZE;
            // unlock_page(); /// I think we should unlock the page here
            page.decreaseRefCount();
        }

        *ppos = index * PAGE_SIZE + offset;
        inode->update_atime(); // update atime field and set inode as dirty
    }

    file_read_actor(read_descriptor_t* desc, pagedesc, offset, nr)
    {
        memcpy(desc->buf, pagedesc.page->getKernelAddr() + offset, nr); // Can page fault/reschedule
        desc->count -= nr;
        desc->written + = nr;
        desc->buf += nr;
    }

    aio_read(reqs, buf, len, pos);
    write(file, buf, count, offset);
    aio_write(req, buf, len, pos);
    readdir(dir, dirent, filldir);
    // poll(file, poll_table);
    ioctl(inode, file, cmd, arg);
    unlocked_ioctl(file, cmd, arg);
    mmap(file, vma);
    open(inode, file);
    flush(file);
    release(inode, file);
    fsync(file, dentry, flag);
    // aio_fsync(req, flag);
    // readv(file, vector, count, offset);
    // writev(file, vector, count, offset);
};

struct DentryCache
{
    std::vector<Dentry*> entryCache;
    std::unordered_map<std::uint64_t, Dentry*> hashTable;
    list<Dentry*> lruDentry;

    // From Dir + pathname to file, call hash to get the hash value of the file, look it into the hashTable
};

struct Fs
{
    int refCount; // nb of process sharing this table
    int umask;    // Bit mask used when opening the file to set the file permissions
    Dentry* root;
    Dentry* pwd;
    vfsmount* rootmnt;
    vfsmount* pwdmnt;
};

struct Files
{
    int refCount; // nb of process sharing this table
    int max_fds;
    int max_fdset;
    int next_fd;
    std::vector<File*> fd;
    std::set<File*> open_fds;
    std::set<File*> close_on_exec_fds;
};

struct FileSystemType
{
    string* name;
    int flags; // fs type flags
    SuperBlock _superblockList[];

    get_sb(flags, dev_name, (void*)data); // reading a sb
    kill_sb();                            // removing a sb
};

kernel::FileSystemType[];

struct fs_namespace
{
    int refCount; // nb of process sharing this table
    vfsmount* root;
    list<vfsmount*> list;
};

struct vfsmount
{
    int refCount; // Usage counter (increased to forbid filesystem unmounting).
    int flags;
    vfsmount* parent;
    Dentry* mountpoint;
    Dentry* root;
    SuperBlock* sb;
    list<vfsmount*> children;
    char* devname;
    fs_namespace* fs_namespace;
};

sys_mount(pathname_dev_file, pathname_mount_point, filesystemtype, mount_flags, (void*)data)

    struct nameidata
{
    Dentry* dentry;
    vfsmount* mnt;
    qstr last;
    int flags;
    int last_type;
    int depth;            // Symlink depth
    char[] * saved_names; // array of pathnames for symlinks
};

struct BlockDesc
{};

struct PageDesc
{
    Page* page;
    std::size_t index;
    bool upToDate;
};

struct block_desc
{
    std::size_t blockNum;
    bool upToDate;
    PageDesc* page;
};

struct biov
{
    std::size_t blkStart;
    std::size_t blkCount;
    Page* page;
    size_t pageOffset;
    std::function<void()> end_io;
};

struct bio
{
    list<biov> list;
};

struct FileAddresSpace
{
    Inode* host;
    std::set<PageDesc*> pages; // (use index (relative to position in file) in page desc as sort key)

    void readpage(std::size_t pageIndex)
    {
        std::blocksInPage = PAGE_SIZE >> host->i_blkbits;
        std::firstBlock = pageIndex * blocksInPage;
        block_desc* blockHead = new block_desc[8];
        for (size_t i = 0; i < blocksInPage; ++i) {
            blockHead[i] = getBlock(firstBlock + i);
        }

        bio io_req_list;
        int start = blockHead[0];
        int size = 1;
        for (auto i = 0; i <= blocksInPage; ++i) {
            if (i < blocksInPage && blockHead[i].blockNum + 1 = blockHead[i + 1].blockNum) {
                size++;
                continue;
            } else {
                biov io_req(start, size, pages[pageIndex], blockSize * (i - size),
                            []()
                            {
                                for (auto i = start; i < start + size; i++) {
                                    blockHead[i].upToDate = true;
                                }
                                for (auto i = start; i < start + size; i++) {
                                    if (!blockHead[i].upToDate)
                                        return;
                                }
                                page[index].uptodate = true;
                                unlock_page(pages[pageIndex]);
                            });
                io_req.add(io_req);
                i++;
                start = blockHead[i];
                size = 1;
            }
        }
        IOScheduler::submit_bio(host->sb->dev, bio);
        lock_page(pages[pageIndex]);
    }

    writepage();
    sync_page();
    writepages();
    set_page_dirty();
    readpages();
    prepare_write();
    commit_write();
    bmap();
    invalidatepage();
    releasepage();
    direct_IO();

    virtual BlockDesc getBlock(std::size_t blockIndex) {}
};

}
