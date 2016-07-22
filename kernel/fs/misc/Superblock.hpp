#ifndef __SUPERBLOCK__HPP__
#define __SUPERBLOCK__HPP__

class Superblock {
public:
	void write();
	void syncFilesystem();
	int remountFilesystem(int flags, char *data);
	int umount();

};

#endif // __SUPERBLOCK__HPP__
