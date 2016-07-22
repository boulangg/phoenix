#ifndef __FS_FILEMANAGER__
#define __FS_FILEMANAGER__

class FileManager {
public:
	/**
	 * combine pathName with workDir if pathName is a relative
	 * path.
	 */
	char* getFullPath(const char* pathName, const char* dir)
	static File* open(const char* path, int flags, int mode);
	static int close(File*);

};

#endif // __FS_FILEMANAGER__
