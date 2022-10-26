//#include <sys/time.h>

#include <stdio.h>

#include "time.h"

int main()
{
	//struct timeval tv;
	//struct timezone tz;

	//int ret = gettimeofday(&tv, &tz);
	tzset();

	char* tzname0 = tzname[0];
	char* tzname1 = tzname[1];
	long tzset = timezone;
	int dl = daylight;
	printf("%i", dl);
	return 0;
}
