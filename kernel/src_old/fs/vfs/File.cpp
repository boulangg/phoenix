/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "File.hpp"

#include "Inode.hpp"

#include <include/constant.h>

/*File::File(Dentry* d) //:
        //dentry(d), pos(0)
{

}*/

File::~File() {}

ssize_t File::write_internal(char*, size_t, loff_t)
{
    return 0;
}
