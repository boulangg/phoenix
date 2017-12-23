
#include <unistd.h>

#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
//#include <syscall/syscall.h>
#include <string.h>
#include <string>

void print_usage() {
	printf("ls [path]");
}

int main(int argc,char* argv[]) {
	if (argc > 2) {
		print_usage();
		return -1;
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
	if (dir == 0) {
		return -1;
	}
	struct dirent* dirp;
	while ((dirp = readdir(dir)) != nullptr) {
		struct stat status;
		const char* name = (char*)dirp->d_name;
		stat(name, &status);
		std::string type_perm("----------");
		if (S_ISDIR(status.st_mode)) {
			type_perm[0] = 'd';
		}
		if (status.st_mode & S_IRUSR) {
			type_perm[1] = 'r';
		}
		if (status.st_mode & S_IWUSR) {
			type_perm[2] = 'w';
		}
		if (status.st_mode & S_IXUSR) {
			type_perm[3] = 'x';
		}
		if (status.st_mode & S_IRGRP) {
			type_perm[4] = 'r';
		}
		if (status.st_mode & S_IWGRP) {
			type_perm[5] = 'w';
		}
		if (status.st_mode & S_IXGRP) {
			type_perm[6] = 'x';
		}
		if (status.st_mode & S_IROTH) {
			type_perm[7] = 'r';
		}
		if (status.st_mode & S_IWOTH) {
			type_perm[8] = 'w';
		}
		if (status.st_mode & S_IXOTH) {
			type_perm[9] = 'x';
		}
		printf("%s\t%i\t%i\t%s\n", type_perm.c_str(), status.st_nlink, status.st_size, name);
	}
	return 0;
}
