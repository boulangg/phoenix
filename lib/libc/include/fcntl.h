#ifndef _FNCTL_H_
#define _FNCTL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/stat.h>
#include <sys/types.h>

#define F_DUPFD
#define F_GETFD
#define F_SETFD
#define F_GETFL
#define F_SETFL
#define F_GETLK
#define F_SETLK
#define F_SETLKW



#define O_RDONLY    0x00000000
#define O_WRONLY    0x00000001
#define O_RDWR      0x00000002
#define O_ACCMODE   0x00000003

#define O_CREAT     0x00000040
#define O_EXCL      0x00000080
#define O_NOCTTY    0x00000100
#define O_TRUNC     0x00000200
#define O_APPEND    0x00000400
#define O_NONBLOCK  0x00000800
#define O_NDELAY    0x00000800
#define O_DSYNC     0x00001000
#define FASYNC      0x00002000
#define O_DIRECT    0x00004000
#define O_DIRECTORY 0x00010000
#define O_NOFOLLOW  0x00020000
#define O_NOATIME   0x00040000
#define O_CLOEXEC   0x00080000
#define O_SYNC      0x00101000
#define O_PATH      0x00200000
#define O_TMPFILE   0x00410000

#define O_LARGEFILE 0x00000000

	struct flock_t
	{
		short l_type;
		short l_whence;
		off_t l_start;
		off_t l_len;
		pid_t l_pid;
	};

	int creat(const char* path, mode_t mode);
	int fcntl(int fd, int cmd, ...);
	int open(const char* pathname, int flags, ...);

#ifdef __cplusplus
}
#endif

#endif
