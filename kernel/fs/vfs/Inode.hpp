#pragma once

#include "Dentry.hpp"
#include "AddressSpace.hpp"
#include "File.hpp"

class SuperBlock;

class Inode {
public:
	virtual ~Inode();

	// super_operations
	//virtual void dirty(int flags);
	//virtual int write();
	//virtual void drop();
	//virtual void clear();

	// inode_operations
	//virtual Dentry* create(Dentry* parent, std::string name);
	virtual Dentry * lookup(Dentry* parent, std::string name);
	//virtual struct dentry * (*lookup) (struct inode *parent, struct dentry *, unsigned int);
	//int (*link) (struct dentry *,struct inode *,struct dentry *);
	//int (*unlink) (struct inode *,struct dentry *);
	//int (*symlink) (struct inode *,struct dentry *,const char *);
	//virtual Dentry* mkdir(umode_t);
	//virtual int rmdir(struct dentry *);
	//virtual int mknod(struct dentry *,umode_t,dev_t);
	//int (*rename) (struct inode *, struct dentry *,
	//		struct inode *, struct dentry *, unsigned int);
	//int (*readlink) (struct dentry *, char __user *,int);
	//const char *(*get_link) (struct dentry *, struct inode *,
	//			 struct delayed_call *);
	//virtual int permission(int);
	//int (*get_acl)(struct inode *, int);
	//int (*setattr) (struct dentry *, struct iattr *);
	//int (*getattr) (const struct path *, struct kstat *, u32, unsigned int);
	//ssize_t (*listxattr) (struct dentry *, char *, size_t);
	//void (*update_time)(struct inode *, struct timespec *, int);
	//int (*atomic_open)(struct inode *, struct dentry *, struct NewFile *,
	//		unsigned open_flag, umode_t create_mode, int *opened);
	//int (*tmpfile) (struct inode *, struct dentry *, umode_t);

	// file_operation
	File* open();

	Inode(SuperBlock* sb, std::uint64_t ino, size_t size);
private:
	virtual File* open_internal() {return nullptr;}

public:
	SuperBlock* sb;
	std::uint64_t ino;
	size_t size;

	//umode_t mode;
	//uid_t uid;
	//gid_t gid;
	//struct timespec atime;
	//struct timespec ctime;
	//struct timespec mtime;
	AddressSpace* mapping;
	std::list<Dentry*> dentries;

	// Help
	//std::uint64_t fileSize;
};


/*
class Inode {
public:
	Inode();
	virtual ~Inode();



	// FILE
	loff_t lseek(loff_t, int32_t);
	ssize_t read(char*, size_t, loff_t*);
	ssize_t write(const char*, size_t, loff_t*);
	uint32_t poll(uint32_t, uint64_t);
	int32_t flush();

	// DIR
	int32_t getdents(struct dirent*, uint32_t);

	int mount(Inode*);
	int unmount();

private:
	uint32_t _refCount;
	uint64_t _inodeNumber;

	Dentry* _path;

	bool _mount;
	Inode* _mountInode;
};

*/
