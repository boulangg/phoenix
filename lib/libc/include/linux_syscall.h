/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef __LINUX_SYSCALL__
#define __LINUX_SYSCALL__

struct linux_dirent64
{
    unsigned long long d_ino; /* Inode number */
    unsigned long long d_off; /* Offset to next linux_dirent */
    unsigned short d_reclen;  /* Length of this linux_dirent */
    unsigned char d_type;
    char d_name[0]; /* Filename (null-terminated) */
};

#endif
