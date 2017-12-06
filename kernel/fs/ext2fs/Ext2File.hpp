
class Ext2File : public File {
public:
	Ext2File();
	virtual ~Ext2File();

	loff_t doLseek(loff_t offset, int32_t origin) override;
	ssize_t doRead(char* buf, ssize_t count) override;
	ssize_t doWrite(const char* buf, ssize_t count) override;
};
