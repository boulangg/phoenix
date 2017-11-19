/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef __DIRECTORYCACHE_HPP__
#define __DIRECTORYCACHE_HPP__

class DirectoryCache {
public:
	DirectoryCache();
	virtual ~DirectoryCache();


private:
	Dentry* root;

};

#endif // __DIRECTORYCACHE_HPP__
