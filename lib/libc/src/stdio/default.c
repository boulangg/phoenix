#include <stdio.h>
#include <stddef.h>
#include <../glue/weak_call.h>

void init_default_std() {
	//stdout = malloc(sizeof(FILE));
}

int fgetc(FILE* stream) {
	if (stream == NULL) {
		return EOF;
	}
	if (stream->mode & _IONBF) {
		char character;
		sys_read(stream->fileno, &character, 1);
		if (character == EOF) {
			// set feof
		}
		return (int)character;
	} else {

	}
	return EOF;
}

int fputc(int character, FILE* stream) {
	if (stream == NULL) {
		return EOF;
	}
	if (stream->mode & _IONBF) {
		return sys_write(stream->fileno, &character, 1);
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
	if (stream != NULL) {
		if (stream->writeBufStart < stream->writeBufPos) {
			int size = stream->writeBufPos-stream->writeBufStart;
			sys_write(stream->fileno, stream->writeBufStart, size);
			stream->writeBufPos = stream->writeBufStart;
			return 0;
		}
	}
	return 0;
}
