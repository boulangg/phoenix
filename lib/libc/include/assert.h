#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef NDEBUG
#define assert(ignore) ((void)0)
#else

#ifdef __cplusplus
extern "C" {
#endif

#define	assert(expression) \
	if ((expression) == false) { \
		fprintf(stderr, "%s:%i %s: Assertion `%s' failed.\n", __FILE__, __LINE__, __FUNCTION__, #expression); \
		exit(EXIT_FAILURE); \
	}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // NDEBUG
