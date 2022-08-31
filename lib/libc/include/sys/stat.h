/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef __SYS_STAT_H__
#define __SYS_STAT_H__

#include <stdint.h>
#include <sys/types.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif


/*struct stat {
	dev_t     st_dev;
	ino_t     st_ino;
	mode_t    st_mode;
	nlink_t   st_nlink;
	uid_t     st_uid;
	gid_t     st_gid;
	dev_t     st_rdev;
	off_t     st_size;
	time_t    st_atime;
	int32_t   st_spare1;
	time_t    st_mtime;
	int32_t   st_spare2;
	time_t    st_ctime;
	int32_t   st_spare3;
	int32_t   st_blksize;
	int32_t   st_blocks;
	uint32_t  st_flags;
	uint32_t  st_gen;
};*/

	struct stat
	{
		dev_t   st_dev;
		ino_t   st_ino;
		nlink_t st_nlink;

		mode_t  st_mode;
		uid_t   st_uid;
		gid_t   st_gid;

		dev_t   st_rdev;
		off_t   st_size;
		size_t  st_blksize;
		size_t  st_blocks;

		struct timespec	st_atime;

		struct timespec	st_mtime;

		struct timespec	st_ctime;
	};

	/* File type */
#define	S_IFMT   00170000

#define	S_IFIFO  00010000
#define	S_IFCHR  00020000
#define	S_IFDIR  00040000
#define	S_IFBLK  00060000
#define	S_IFREG  00100000
#define	S_IFLNK  00120000
#define	S_IFSOCK 00140000

#define S_ISFIFO(m) ((m & S_IFMT) == S_IFIFO)
#define S_ISCHR(m)  ((m & S_IFMT) == S_IFCHR)
#define S_ISDIR(m)  ((m & S_IFMT) == S_IFDIR)
#define S_ISBLK(m)  ((m & S_IFMT) == S_ISBLK)
#define S_ISREG(m)  ((m & S_IFMT) == S_IFREG)
#define S_ISLNK(m)  ((m & S_IFMT) == S_IFLNK)
#define S_ISSOCK(m) ((m & S_IFMT) == S_IFSOCK)

/* File mode bits */
#define S_IRWXU 00000700
#define S_IRUSR 00000400
#define S_IWUSR 00000200
#define S_IXUSR 00000100

#define S_IRWXG 00000070
#define S_IRGRP 00000040
#define S_IWGRP 00000020
#define S_IXGRP 00000010

#define S_IRWXO 00000007
#define S_IROTH 00000004
#define S_IWOTH 00000002
#define S_IXOTH 00000001

	int chmod(const char*, mode_t);
	int fchmod(int, mode_t);
	int fstat(int, struct stat*);
	int lstat(const char*, struct stat*);
	int mkdir(const char*, mode_t);
	int mkfifo(const char*, mode_t);
	int mknod(const char*, mode_t, dev_t);
	int stat(const char*, struct stat*);
	mode_t umask(mode_t);

#ifdef __cplusplus
}
#endif


#endif // __SYS_STAT_H__
