#include "sys.h"

#include <stddef.h>
#include "sys/auxv.h"

char** environ;
auxv_t* __auxv_ptr;

void init_sys(char** envp) {
	environ = envp;

	size_t i = 0;
	while(environ[i] != 0) {
		i++;
	}

	__auxv_ptr = (auxv_t*)(envp + i + 1);
}
