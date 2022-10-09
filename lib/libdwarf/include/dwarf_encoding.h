#include <stdint.h>
#include <stddef.h>

uint64_t decodeUnsignedInt(const char* input, size_t* index);
int64_t decodeSignedInt(const char* input, size_t* index);
