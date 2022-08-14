/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include <stdio.h>
#include <unistd.h>

int main(int argc,char* argv[]) {
	(void)argc;
	(void)argv;
	fprintf(stdout, "**%s**\n", "ok");
	fprintf(stdout, "**%c**\n", 'a');
	fprintf(stdout, "**% i**\n", 1523);
	fprintf(stdout, "**%#p**\n", &argv);
	fprintf(stdout, "**%#7.3x**\n", 1);

	return getpid();
}
