/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef __SUPERBLOCK_HPP__
#define __SUPERBLOCK_HPP__

class Superblock {
public:
	Superblock();
	virtual ~Superblock();

	Inode* allocInode();
	void destroyInode(Inode*);

};

#endif // __SUPERBLOCK_HPP__
