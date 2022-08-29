#include <driver/PCI/IDE/IDEStruct.hpp>

template<int s, int t> struct check_size
{
	static_assert(s == t, "wrong size");
};

#define offsetof(st, m) __builtin_offsetof(st, m)

check_size<sizeof(IDEIdentifyData), 512> test1;
check_size<offsetof(IDEIdentifyData, logicalSectorSize), 234> test2;

static_assert (sizeof(IDEIdentifyData) == 512, "Size of IDEIdentifyData struct is not correct");

//static_assert (offsetof(logicalSectorSize) == 117, "incorrect offset for logicalSectorSize");
