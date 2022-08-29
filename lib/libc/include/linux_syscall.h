#ifndef __LINUX_SYSCALL__
#define __LINUX_SYSCALL__

struct linux_dirent64
{
	unsigned long long d_ino;     /* Inode number */
	unsigned long long d_off;     /* Offset to next linux_dirent */
	unsigned short     d_reclen;  /* Length of this linux_dirent */
	unsigned char      d_type;
	char               d_name[0]; /* Filename (null-terminated) */
};

#endif
