#include "cstring"

namespace std {

void* memcpy(void* dest, const void* src, std::size_t count)
{
	return ::memcpy(dest, src, count);
}

}
