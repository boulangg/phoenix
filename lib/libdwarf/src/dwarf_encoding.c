#include "dwarf_encoding.h"

#include <stdbool.h>

uint64_t decodeUnsignedInt(const char* input, size_t* index)
{
	uint64_t result = 0;
	uint8_t shift = 0;
	while (true) {
		uint8_t byte = input[(*index)++];
		;
		result |= ((byte & 0x7f) << shift);
		shift += 7;
		if ((byte & 0x80) == 0) {
			break;
		}
	}
	return result;
}

int64_t decodeSignedInt(const char* input, size_t* index)
{
	int64_t result = 0;
	uint8_t shift = 0;
	uint8_t size = sizeof(int64_t) * 8;
	uint8_t byte = 0;
	while (true) {
		byte = input[(*index)++];
		result |= ((byte & 0x7f) << shift);
		shift += 7;
		/* sign bit of byte is second high order bit (0 x40 ) */
		if ((byte & 0x80) == 0) {
			break;
		}
	}
	if ((shift < size) && ((byte & 0x40) != 0)) {
		/* sign extend */
		result |= (-1ll << shift);
	}
	return result;
}
