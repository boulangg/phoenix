#pragma once

#include <string>
#include <vector>

#include "Dentry.hpp"

class DentryCache {
public:
	static Dentry* findDentry(Dentry* parent, std::vector<std::string> pathname, size_t index);

};

/*class DentryCache {

	dget: open a new handle for an existing dentry (this just increments
			the usage count)

	dput: close a handle for a dentry (decrements the usage count). If
	the usage count drops to 0, and the dentry is still in its
	parent's hash, the "d_delete" method is called to check whether
	it should be cached. If it should not be cached, or if the dentry
	is not hashed, it is deleted. Otherwise cached dentries are put
	into an LRU list to be reclaimed on memory shortage.

	d_drop: this unhashes a dentry from its parents hash list. A
	subsequent call to dput() will deallocate the dentry if its
	usage count drops to 0

	d_delete: delete a dentry. If there are no other open references to
	the dentry then the dentry is turned into a negative dentry
	(the d_iput() method is called). If there are other
	references, then d_drop() is called instead

	d_add: add a dentry to its parents hash list and then calls
	d_instantiate()

	d_instantiate: add a dentry to the alias hash list for the inode and
	updates the "d_inode" member. The "i_count" member in the
	inode structure should be set/incremented. If the inode
	pointer is NULL, the dentry is called a "negative
	dentry". This function is commonly called when an inode is
	created for an existing negative dentry

	d_lookup: look up a dentry given its parent and path name component
	It looks up the child of that given name from the dcache
	hash table. If it is found, the reference count is incremented
	and the dentry is returned. The caller must use dput()
	to free the dentry when it finishes using it.
};*/
