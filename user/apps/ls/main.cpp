
#include <unistd.h>

#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <syscall/syscall.h>
#include <string.h>

void print_usage() {
	printf("ls [path]");
}

int main(int argc,char* argv[]) {
	if (argc >= 2) {
		print_usage();
	}
	const char* path;
	if (argc == 1) {
		path = ".";
	} else {
		path = argv[1];
	}
	/*int fd = sys_open(".", O_DIRECTORY, O_RDONLY);
	char buffer[4096];
	int bytesread;
	while((bytesread = sys_getdents64(fd, (struct linux_dirent64*)buffer, 4096)) != 0) {
		for (int i = 0; i < bytesread;) {
			struct linux_dirent64* dir = (struct linux_dirent64 *)(buffer + i);
			char* name = dir->d_name;
			printf("%i: %s\n", dir->d_ino, (char*)dir->d_name);
			i += dir->d_reclen;
		}
	}*/
	DIR* dir = opendir(path);
	struct dirent* dirp;
	while ((dirp = readdir(dir)) != nullptr) {
		printf("%i: %s\n", dirp->d_ino, (char*)dirp->d_name);;
	}
	return 0;
}
