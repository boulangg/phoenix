#include <time.h>

time_t time(time_t* tloc)
{
	return sys_time(tloc);
}
