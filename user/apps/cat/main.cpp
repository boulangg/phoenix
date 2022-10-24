#include <stdio.h>
#include <string>

#define BUFFER_SIZE 256

int main(int argc, char* argv[])
{
	(void)argc;
	std::string filename = argv[1];
	if (filename.empty()) {
		filename = "Makefile";
	}
	FILE* fd = fopen(filename.c_str(), "r");
	char buffer[BUFFER_SIZE];
	while (fgets(buffer, BUFFER_SIZE - 1, fd) != nullptr) {
		fgets(buffer, BUFFER_SIZE - 1, fd);
		printf("%s", buffer);
	}
}
