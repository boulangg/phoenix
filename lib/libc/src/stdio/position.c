
#include "io.h"

int fgetpos(FILE* str, fpos_t* pos)
{
	CHECK_FILE(str);
	*pos = ftell(str);
	return 0;
}

int fseek(FILE* str, long int offset, int origin)
{
	CHECK_FILE(str);
	(void)str;
	(void)offset;
	(void)origin;
	switch (origin) {
	case SEEK_SET:
		break;
	case SEEK_CUR:
		break;
	case SEEK_END:
		break;
	default:
		return EOF;
	}

	//str->bufVirtPos = BUF_VIRT_SIZE;
	return -1;
}

int fsetpos(FILE* str, const fpos_t* pos)
{
	CHECK_FILE(str);
	(void)str;
	(void)pos;
	return fseek(str, *pos, SEEK_SET);
}

long int ftell(FILE* str)
{
	CHECK_FILE(str);
	return str->offset + (str->bufPos - str->bufStart) + BUF_VIRT_SIZE - str->bufVirtPos;;

}

void rewind(FILE* str)
{
	fseek(str, 0, SEEK_SET);
}
