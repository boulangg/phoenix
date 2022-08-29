#include <dirent.h>

#include <syscall/syscall.h>

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#define READDIR_BUF_SIZE 4096

#define min(X, Y) (X < Y ? X : Y)

typedef struct DIR
{
	ino_t d_ino;
	char d_name[NAME_MAX + 1];
	int fd;
	char readdirBuffer[READDIR_BUF_SIZE];
	size_t readdirPos;
	size_t readdirSize;
	struct dirent dirp;

} DIR;

int closedir(DIR* dir)
{
	close(dir->fd);
	free(dir);
	return 0;
}

DIR* opendir(const char* pathname)
{
	int fd = open(pathname, O_DIRECTORY, 0);
	if (fd < 0) {
		return NULL;
	}
	DIR* dir = malloc(sizeof(DIR));
	dir->fd = fd;
	// TODO fill ino (stat required)
	size_t name_size = min(NAME_MAX, strlen(pathname));
	memcpy(dir->d_name, pathname, name_size);
	dir->readdirPos = 0;
	dir->readdirSize = 0;
	return dir;
}

struct dirent* readdir(DIR* dir)
{
// TODO CHECK_DIR(dir);
	struct dirent* dirp;
	readdir_r(dir, &dir->dirp, &dirp);
	return dirp;
}

int readdir_r(DIR* dir, struct dirent* entry, struct dirent** result)
{
// TODO CHECK_DIR(dir);
	if (dir->readdirPos == dir->readdirSize) {
		int ret = sys_getdents64(dir->fd, (struct linux_dirent64*)(dir->readdirBuffer), READDIR_BUF_SIZE);
		if (ret < 0) {
			*result = NULL;
			return ret;
		} else {
			dir->readdirSize = ret;
			dir->readdirPos = 0;
		}
	}
	if (dir->readdirPos != dir->readdirSize) {
		struct linux_dirent64* dirp = (struct linux_dirent64*)(dir->readdirBuffer + dir->readdirPos);
		entry->d_ino = dirp->d_ino;
		size_t name_size = min(NAME_MAX, strlen(dirp->d_name) + 1);
		memcpy(entry->d_name, dirp->d_name, name_size);
		dir->readdirPos += dirp->d_reclen;
		*result = entry;
	} else {
		*result = NULL;
	}
	return 0;
}

void rewinddir(DIR* dir)
{
// TODO CHECK_DIR(dir);
	sys_llseek(dir->fd, 0, SEEK_SET);
}

void seekdir(DIR* dir, long int offset)
{
// TODO CHECK_DIR(dir);
	sys_llseek(dir->fd, offset, SEEK_SET);
	dir->readdirSize = 0;
	dir->readdirPos = 0;
}

long int telldir(DIR* dir)
{
// TODO CHECK_DIR(dir)
	return sys_llseek(dir->fd, 0, SEEK_CUR);
}
