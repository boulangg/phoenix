#include <stdio.h>

#define BUFFER_SIZE 256

int main(int argc, char* argv[])
{
	char* filename = argv[1];
	if (filename == NULL) {
		filename = "Makefile";
	}
	FILE* fd = fopen(filename, "r");
	char buffer[BUFFER_SIZE];
	while (fgets(buffer, BUFFER_SIZE - 1, fd) != nullptr) {
		fgets(buffer, BUFFER_SIZE - 1, fd);
		printf("%s", buffer);
	}
}
