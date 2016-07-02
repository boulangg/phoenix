/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef __TIME_H__
#define __TIME_H__


#ifdef __cplusplus
extern "C" {
#endif

struct tm {
	int tm_sec;     // seconds [0,61]
	int tm_min;     // minutes [0,59]
	int tm_hour;    // hour [0,23]
	int tm_mday;    // day of month [1,31]
	int tm_mon;     // month of the year [0,11]
	int tm_year;    // years since 1900
	int tm_wday;    // day of week [0,6] (sunday = 0)
	int tm_yday;    // day of year [0,365]
	int tm_isdst;   // daylight saving flag
};

#include <def/def_null.h>
//#define CLK_TCK
#define CLOCKS_PER_SEC 1000000

#include <sys/types.h>

struct timespec {
	time_t tv_sec;
	long tv_nsec;
};

struct itimerspec {
	struct timespec it_interval;  // timer period
	struct timespec it_value;     // timer expiration
};

//#define CLOCK_REALTIME
//#define TIMER_ABSTIME

extern int getdate_err;

char      *asctime(const struct tm *);
char      *asctime_r(const struct tm *, char *);
clock_t    clock(void);
int        clock_getres(clockid_t, struct timespec *);
int        clock_gettime(clockid_t, struct timespec *);
int        clock_settime(clockid_t, const struct timespec *);
char      *ctime(const time_t *);
char      *ctime_r(const time_t *, char *);
double     difftime(time_t, time_t);
struct tm *getdate(const char *);
struct tm *gmtime(const time_t *);
struct tm *gmtime_r(const time_t *, struct tm *);
struct tm *localtime(const time_t *);
struct tm *localtime_r(const time_t *, struct tm *);
time_t     mktime(struct tm *);
int        nanosleep(const struct timespec *, struct timespec *);
size_t     strftime(char *, size_t, const char *, const struct tm *);
char      *strptime(const char *, const char *, struct tm *);
time_t     time(time_t *);
//int        timer_create(clockid_t, struct sigevent *, timer_t *);
int        timer_delete(timer_t);
int        timer_gettime(timer_t, struct itimerspec *);
int        timer_getoverrun(timer_t);
int        timer_settime(timer_t, int, const struct itimerspec *,
               struct itimerspec *);
void       tzset(void);

extern int       daylight;
extern long int  timezone;
extern char     *tzname[];

#ifdef __cplusplus
}
#endif

#endif // __TIME_H__
