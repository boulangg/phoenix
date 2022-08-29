#include "Dentry.hpp"

#include "Inode.hpp"
#include "File.hpp"
#include "SuperBlock.hpp"

Dentry::Dentry(Dentry* parent, Inode* inode, std::string name) :
	inode(inode), parent(parent), name(name), children(), mount(nullptr)
{

}

Dentry::Dentry(Inode* inode) :
	inode(inode), parent(this), name("/"), mount(nullptr)
{
	//Dentry* d = this;
	//parent->children.push_back(d);
}


Dentry::~Dentry()
{
//parent->children.remove(this);
}

bool Dentry::isRoot()
{
	return this == inode->sb->getRoot();
}

/*File* Dentry::open() {
	//File* f = inode->open();
	//f->dentry = this;
	//f->count++;
	return doOpen();
}*/
