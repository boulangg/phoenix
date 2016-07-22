#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/*#define EOF (-1)

#define BUFSIZ 0x1000
#define _IONBF 0x1
#define _IOLBF 0x2
#define _IOFBF 0x4*/

// Flags
#define BUFWRITE 0x1
#define USERBUF  0x2
#define MAGIC_MASK   0xFFFF0000
#define MAGIC_VALUE  0xABCD0000

typedef int (*fgetc_fn)(FILE* str);
typedef int (*fputc_fn)(int c, FILE* str);
//typedef int (*fflush_fn)(FILE* str);

typedef struct stream_ops {
	fgetc_fn fgetc;
	fputc_fn fputc;
	//fflush_fn fflush;
} stream_ops;

struct stream_ops str_fn;
struct stream_ops file_fn;

struct FILE {
	uint64_t flags;
	char* bufStart;
	char* bufPos;
	char* bufEnd;
	size_t bufSize;
	int64_t fileno;
	int64_t mode;
	int64_t offset;
	bool eof;
	int error;
	stream_ops* fn;
};


#define CHECK_FILE(str) {if (str == NULL) return EOF;                            \
                         if ((str->flags & MAGIC_MASK) != MAGIC_VALUE) return EOF; \
                         if (str->fn == NULL) return EOF;}

void init_io();
