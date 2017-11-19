
class Ext2File : public File {
public:
	Ext2File();
	virtual ~Ext2File();

	loff_t lseek(loff_t offset, int32_t origin);
	ssize_t read(char* buf, ssize_t count);
	ssize_t write(const char* buf, ssize_t count);
};
