#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <errno.h>
#include <def/def_null.h>

#include <unistd.h>

void init_default_std() {
	//stdout = malloc(sizeof(FILE));
}

FILE * fopen(const char* filename, const char* mode) {
	int flags = 0;
	switch(mode[0]) {
	case 'r':
		if (mode[1] == '+') {
			flags |= O_RDWR;
		} else {
			flags |= O_RDONLY;
		}
		break;
	case 'w':
		flags |= O_TRUNC | O_CREAT;
		if (mode[1] == '+') {
			flags |= O_RDWR;
		} else {
			flags |= O_WRONLY;
		}
		break;
	case 'a':
		flags |= O_CREAT;
		if (mode[1] == '+') {
			flags |= O_RDWR;
		} else {
			flags |= O_WRONLY;
		}
		break;
	default:
		return NULL;
	}
	int fileMode = 0x0777;

	int fd = open(filename, flags, fileMode);
	if (fd < 0) {
		errno = fd;
		return NULL;
	}
	FILE* f = malloc(sizeof(FILE));
	f->fileno = fd;
	f->flags = flags;
	f->mode = _IONBF;
	f->offset = 0;
	f->readBufStart = malloc(BUFSIZ);
	f->readBufPos = f->readBufStart;
	f->readBufEnd = f->readBufStart;
	f->readBufSize = BUFSIZ;
	f->writeBufStart = malloc(BUFSIZ);
	f->writeBufPos = f->writeBufStart;
	f->writeBufEnd = f->writeBufStart + BUFSIZ;
	f->writeBufSize = BUFSIZ;
	f->eof = false;
	f->error = 0;
	return f;
}

int fclose(FILE* stream) {
	fflush(stream);
	free(stream->readBufStart);
	free(stream->readBufEnd);
	return close(stream->fileno);
}

int feof(FILE* stream) {
	return stream->eof;
}

int ferror(FILE* stream) {
	return stream->error;
}

void clearerr(FILE* stream) {
	stream->error = 0;
	stream->eof = 0;
}

int fgetc(FILE* stream) {
	if (stream == NULL) {
		return EOF;
	}
	if (feof(stream)) {
		return EOF;
	}
	if (stream->mode & _IONBF) {
		char character;
		read(stream->fileno, &character, 1);
		if (character == EOF) {
			stream->eof = true;
		}
		return (int)character;
	} else {
		if (stream->readBufPos == stream->readBufPos) {
			// TODO
		}
	}
	return EOF;
}

int fputc(int character, FILE* stream) {
	if (stream == NULL) {
		return EOF;
	}
	if (stream->mode & _IONBF) {
		return write(stream->fileno, &character, 1);
	} else {
		stream->writeBufPos[0] = character;
		stream->writeBufPos++;
		if (stream->writeBufPos == stream->writeBufEnd) {
			fflush(stream);
		} else if ((stream->mode &  _IOLBF)
				&& (character == '\n')) {
			fflush(stream);
		}
	}
	return 0;
}

int fflush(FILE* stream) {
	if (stream == NULL) {
		return EOF;
	}
	if (stream->writeBufStart < stream->writeBufPos) {
		int size = stream->writeBufPos-stream->writeBufStart;
		write(stream->fileno, stream->writeBufStart, size);
		stream->writeBufPos = stream->writeBufStart;
	}
	return 0;
}

char* fgets(char* str, int num, FILE* stream) {
	if (stream == NULL) {
		return NULL;
	}
	char c = fgetc(stream);
	int i = 0;
	while (c != EOF && c != '\n' && i < num - 1) {
		str[i] = c;
		i++;
		c = fgetc(stream);
	}
	if (feof(stream) && (i == 0)) {
		return NULL;
	}
	str[i+1] = '\0';
	return str;
}

int fputs(const char* str, FILE* stream) {
	if (stream == NULL) {
		return EOF;
	}
	int i = 0;
	while (str[i] != '\0') {
		int ret = fputc(str[i], stream);
		if (ret != 0) {
			return EOF;
		}
		i++;
	}
	return i;
}

void setvbuf(FILE* stream, char* buffer, int mode, size_t size) {
	(void)stream;
	(void)buffer;
	(void)mode;
	(void)size;
	// TODO
}

void setbuf(FILE * stream, char* buffer) {
	if (buffer == NULL) {
		setvbuf(stream, buffer, _IONBF, BUFSIZ);
	} else {
		setvbuf(stream, buffer, _IOFBF, BUFSIZ);
	}
}

