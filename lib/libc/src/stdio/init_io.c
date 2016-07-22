
#include "io.h"

void init_io() {
	stdout = malloc(sizeof(FILE));
	stdout->fileno = 1;
	stdout->flags = MAGIC_VALUE;
	stdout->fn = &file_fn;
	//stdout->mode = _IONBF;		// TODO Change to line buffer (or full buffer)
	setvbuf(stdout, NULL, _IONBF, BUFSIZ);
	stdout->eof = false;
}
