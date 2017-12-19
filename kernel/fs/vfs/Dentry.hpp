#pragma once

#include <list>
#include <string>

class Inode;
class File;

class Dentry {
public:
	// d_release
	virtual ~Dentry();

	// super_operations
	//virtual int statfs(struct kstatfs *);

	// dentry_operations
	//virtual int revalidate(unsigned int);
	//virtual int weakRevalidate(unsigned int);
	//virtual int hash(struct qstr *);
	//virtual int compare(unsigned int, const char *, const struct qstr *);
	//virtual int cached(); // d_delete

	//void (*d_iput)(struct dentry *, struct inode *);

	//virtual char* dname(char * buffer, int buflen);

	//struct vfsmount *(*d_automount)(struct path *);
	//int (*d_manage)(const struct path *, bool);
	//struct dentry *(*d_real)(struct dentry *, const struct inode *,
	//		unsigned int, unsigned int);

	//Dentry* getParent() const;
	//void setParent(Dentry* parent);

	//Inode* getInode() const;
	//void setInode(Inode* inode);

	std::string getPathName() {
		if (parent == this) {
			return "/";
		}
		return parent->getPathName() + "/" + name;
	}

	virtual Inode* getInode() {
		return inode;
	}

	// d_init
	Dentry(Dentry* parent, Inode* inode, std::string name);

//protected:
	// Only for root nodes
	Dentry(Inode* inode);

	/*virtual File* open() {
		return nullptr;
	}*/
	//virtual File* doOpen();

//private:
public:
	Inode* inode;
	Dentry* parent;
	std::string name;
	//SuperBlock* sb;
	std::list<Dentry*> children;
	Dentry* mount;
};

