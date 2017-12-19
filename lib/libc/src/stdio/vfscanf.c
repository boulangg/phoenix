/*
 * Copyright (c) 2017 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "io.h"

#include <ctype.h>
#include <stdarg.h>
//#include <stdlib.h>

// A format specifier follow the following prototype:
// %[*][width][length]specifier

// Supported length options
enum ARG_LENGTH {
	LENGTH_NONE,	// "(none)"
	LENGTH_HH,		// "hh"
	LENGTH_H,		// "h"
	LENGTH_L,		// "l"
	LENGTH_LL,		// "ll"
	LENGTH_J,		// "j"
	LENGTH_Z,		// "z"
	LENGTH_T,		// "t"
	LENGTH_L_MAJ	// "L"
};

// Supported specifiers
enum ARG_SPEC {
	SPEC_INT,				// "i"
	SPEC_INT_DEC,			// "d"
	SPEC_UINT_DEC,			// "u"
	SPEC_UINT_OCT,			// "o"
	SPEC_UINT_HEX,			// "x"
	SPEC_FLOAT,				// "f", "e", "g", "a"
	SPEC_CHAR,				// "c"
	SPEC_STR,				// "s"
	SPEC_PTR,				// "p"
	SPEC_SCANSET,			// "[characters]"
	SPEC_NEG_SCANSET, 		// "[^characters]"
	SPEC_NOTHING,			// "n"
	SPEC_PERCENT,			// "%"
	SPEC_ERROR				// "(invalid)"
};

// Supported width options
// "(number)"
// "*"

// Supported precision options
// ".(number)"
// ".*"

struct arg_info {
	bool ignored;
	int width;
	enum ARG_LENGTH length;
	enum ARG_SPEC specifier;
};

int hexToInt(char c) {
	if (isdigit(c)) {
		return c - '0';
	} else if (c >= 'a' && c <= 'f') {
		return c - 'a' + 10;
	} else if (c >= 'A' && c <= 'F') {
		return c - 'A' + 10;
	} else {
		return 0;
	}
}


int vfscanf(FILE* str, const char* format, va_list arg) {
	CHECK_FILE(str);
	// TODO
	(void)str;
	(void)format;
	(void)arg;
	return -1;
}
