#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/auxv.h>

int main(int argc, char* argv[], char* envp[])
{
	(void)argc;
	(void)argv;
	int envpCount = 0;
	while (envp[envpCount] != NULL) {
		printf("%s\n", envp[envpCount]);
		envpCount++;
	}
	printf("\n");

	const char* username = getenv("USERNAME");
	if (username != NULL) {
		printf("USERNAME: %s\n", username);
	}

	uint64_t val = getauxval(AT_PHDR);
	printf("AT_PHDR: %p\n", (void*)val);

	return 0;
}
