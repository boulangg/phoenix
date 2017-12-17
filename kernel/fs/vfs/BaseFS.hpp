#pragma once

#include <unistd.h>

#include "File.hpp"
#include "Inode.hpp"
#include "Dentry.hpp"
#include "SuperBlock.hpp"
#include "AddressSpace.hpp"

template <class SuperBlock, class Inode, class Dentry, class File, class AddressSpace>
class BaseFSInfo {
public:
	typedef SuperBlock superblock_t;
	typedef Inode inode_t;
	typedef Dentry dentry_t;
	typedef File file_t;
	typedef AddressSpace address_space_t;
};


template <class FSInfo>
class BaseFile : public File {
public:
	typedef typename FSInfo::superblock_t superblock_t;
	typedef typename FSInfo::inode_t inode_t;
	typedef typename FSInfo::dentry_t dentry_t;
	typedef typename FSInfo::file_t file_t;
	typedef typename FSInfo::address_space_t address_space_t;


	BaseFile() {

	}

	BaseFile(inode_t*inode) : File() {
		_inode = inode;
	}

	BaseFile(dentry_t* dentry, inode_t* inode, size_t size) : File(),
		_dentry(dentry), _inode(inode), _size(0) {

	}

	virtual ~BaseFile() {

	}

	virtual Dentry* getDentry() override {
		return (Dentry*)_dentry;
	}

	virtual Inode* getInode() override {
		return _inode;
	}

	virtual int64_t doLseek(int64_t offset, uint32_t origin) override {
		// TODO check overflows
		switch(origin) {
		case SEEK_SET:
			_pos = offset;
			break;
		case SEEK_CUR:
			_pos += offset;
			break;
		case SEEK_END:
			_pos = _size + offset;
			break;
		default:
			return -1;
		}

		if (_pos > (int64_t)_size) {
			_pos = _size;
		}
		if (_pos < 0) {
			_pos = 0;
		}
		return 0;
	}

	virtual ssize_t doRead(char* buffer, size_t size, loff_t offset) override {
		size_t i;
		for (i = offset; i < offset + size && i < _inode->size; i++) {
			size_t pageNo = i / PAGE_SIZE;
			Page* p = _inode->mapping->getPage(pageNo);
			buffer[i] = ((char*)p->kernelMappAddr)[i%PAGE_SIZE];
		}
		return i-offset;
	}

	/*virtual int doMmap(struct VirtualArea *area) override {

	}*/

//protected:
	dentry_t* _dentry;
	inode_t* _inode;
	size_t _size;
};

template <class FSInfo>
class BaseDentry : public Dentry {
public:
	typedef typename FSInfo::superblock_t superblock_t;
	typedef typename FSInfo::inode_t inode_t;
	typedef typename FSInfo::dentry_t dentry_t;
	typedef typename FSInfo::file_t file_t;
	typedef typename FSInfo::address_space_t address_space_t;

	BaseDentry(inode_t* inode) : Dentry((Inode*)inode) {
		//dentry_t* d = this;
		//_inode = inode;
		//_parent = this;
		//_children.push_back(d);
		//_mount = nullptr;
		_inode = inode;
	}

	BaseDentry(Dentry* parent, inode_t* inode, std::string name) : Dentry(parent, inode, name),
			_inode(inode)
	{

	}

	virtual ~BaseDentry() {

	}

	virtual File* open() override {
		return doOpen();
	}

	virtual file_t* doOpen() {
		file_t* f = _inode->doOpen();
		f->_dentry = (dentry_t*)this;
		//f->count++;
		return f;
	}

protected:
	inode_t* _inode;
	//Dentry* _parent;
	//std::list<dentry_t*> _children;
	//std::string _name;
	//Dentry* _mount;

};

template <class FSInfo>
class BaseInode : public Inode {
public:
	typedef typename FSInfo::superblock_t superblock_t;
	typedef typename FSInfo::inode_t inode_t;
	typedef typename FSInfo::dentry_t dentry_t;
	typedef typename FSInfo::file_t file_t;
	typedef typename FSInfo::address_space_t address_space_t;

	BaseInode(superblock_t* sb, std::uint64_t ino) : Inode((SuperBlock*)sb, ino),
			sb(sb)
	{

	}

	virtual ~BaseInode() {

	}

	/*virtual Dentry* doCreate(Dentry* parent, std::string name) {}*/

	virtual file_t* doOpen() = 0;

	superblock_t* sb;
};

template <class FSInfo>
class BaseSuperBlock : public SuperBlock {
public:
	typedef typename FSInfo::superblock_t superblock_t;
	typedef typename FSInfo::inode_t inode_t;
	typedef typename FSInfo::dentry_t dentry_t;
	typedef typename FSInfo::file_t file_t;
	typedef typename FSInfo::address_space_t address_space_t;

	virtual ~BaseSuperBlock() {

	}

};

template <class FSInfo>
class BaseAddressSpace : public AddressSpace {
public:
	typedef typename FSInfo::superblock_t superblock_t;
	typedef typename FSInfo::inode_t inode_t;
	typedef typename FSInfo::dentry_t dentry_t;
	typedef typename FSInfo::file_t file_t;
	typedef typename FSInfo::address_space_t address_space_t;

	BaseAddressSpace(inode_t* inode) : AddressSpace(inode), _host(inode) {}

	virtual ~BaseAddressSpace() {

	}

	inode_t* _host;
};
