#include <time.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

int daylight;
long int timezone;
char* tzname[];

struct ttinfo
{
	int32_t       tt_utoff;
	unsigned char tt_isdst;
	unsigned char tt_desigidx;
	unsigned char tt_isstd;
	unsigned char tt_isut;
};

struct tt
{
	int32_t tt_time;
	uint8_t tt_type;
};

struct ttleap
{
	uint32_t      tt_start;
	int32_t       tt_leap;
};

static void tzsetUTC()
{
	daylight = 0;
	timezone = 0;
	tzname[0] = "UTC";
	tzname[1] = "UTC";
}

static uint8_t readBEu8(FILE* file)
{
	unsigned char buf[1];
	memset(buf, '\0', 1);
	fread(buf, 1, 1, file);
	return (uint8_t)buf[0];
}

static uint32_t readBEu32(FILE* file)
{
	unsigned char buf[4];
	memset(buf, '\0', 4);
	fread(buf, 1, 4, file);
	return (uint32_t)buf[3] | (uint32_t)buf[2] << 8 | (uint32_t)buf[1] << 16 | (uint32_t)buf[0] << 24;
}

static uint32_t readBEs32(FILE* file)
{
	unsigned char buf[4];
	memset(buf, '\0', 4);
	fread(buf, 1, 4, file);
	return (uint32_t)buf[3] | (uint32_t)buf[2] << 8 | (uint32_t)buf[1] << 16 | (uint32_t)buf[0] << 24;
}

static int tzset_internal(void)
{
	FILE* TZfile = NULL;

	// TODO Check TZ env variable and /etc/timezone

	// Check /etc/localtime
	if (TZfile == NULL) {
		TZfile = fopen("/etc/localtime", "rb");
	}

	if (TZfile == NULL) {
		return -1;
	}

	// Verify tzfile
	char TZfileStart[20];
	fread(TZfileStart, 1, 20, TZfile);
	if (strncmp(TZfileStart, "TZif", 4) != 0) {
		// Not a TZfile
		return -1;
	}
	if (!(TZfileStart[4] == '\0' || TZfileStart[4] == '2' || TZfileStart[4] == '3')) {
		// Format not recognized
		return -1;
	}

	uint32_t tzh_ttisutcnt = readBEu32(TZfile);		// The number of UT/local indicators stored in the file.
	uint32_t tzh_ttisstdcnt = readBEu32(TZfile);	// The number of standard/wall indicators stored in the file.
	uint32_t tzh_leapcnt = readBEu32(TZfile);		// The number of leap seconds for which data entries are stored in the file.
	uint32_t tzh_timecnt = readBEu32(TZfile);		// The number of transition times for which data entries are stored in the file.
	uint32_t tzh_typecnt = readBEu32(TZfile);		// The number of local time types for which data entries are stored in the file(must not be zero).
	uint32_t tzh_charcnt = readBEu32(TZfile);		// The number of bytes of time zone abbreviation strings stored in the file.

	time_t currentTime = time(NULL);
	uint32_t ttIndex = 0;
	for (uint32_t i = 0; i < tzh_timecnt; i++) {
		int32_t tt = readBEs32(TZfile);
		if (tt > currentTime) {
			ttIndex = i - 1;
			break;
		}
	}

	fseek(TZfile, 20 + 6 * 4 + 4 * tzh_timecnt + 1 * ttIndex, SEEK_SET);
	uint8_t ttType = readBEu8(TZfile);

	fseek(TZfile, 20 + 6 * 4 + 5 * tzh_timecnt + 6 * ttType, SEEK_SET);

	int32_t tt_utoff = readBEs32(TZfile);
	daylight = readBEu8(TZfile);
	timezone = readBEu8(TZfile);




	for (uint32_t i = 0; i < tzh_timecnt; i++) {
		int32_t tt = readBEs32(TZfile);
		if (tt > currentTime) {
			ttIndex = i - 1;
			break;
		}
	}


	struct tt* ttTable = calloc(tzh_timecnt, sizeof(struct tt));
	for (int i = 0; i < tzh_timecnt; i++) {
		ttTable[i].tt_time = readBEs32(TZfile);
	}
	for (int i = 0; i < tzh_timecnt; i++) {
		ttTable[i].tt_type = readBEu8(TZfile);
	}

	struct ttinfo* ttInfoTable = calloc(tzh_typecnt, sizeof(struct ttinfo));
	for (int i = 0; i < tzh_typecnt; i++) {
		ttInfoTable[i].tt_utoff = readBEs32(TZfile);
		ttInfoTable[i].tt_isdst = readBEu8(TZfile);
		ttInfoTable[i].tt_desigidx = readBEu8(TZfile);
	}

	struct ttleap* ttLeapTable = calloc(tzh_leapcnt, sizeof(struct ttleap));
	for (int i = 0; i < tzh_leapcnt; i++) {
		ttLeapTable[i].tt_start = readBEu32(TZfile);
		ttLeapTable[i].tt_leap = readBEs32(TZfile);
	}

	char* str = malloc(tzh_charcnt);
	fread(str, tzh_charcnt, 1, TZfile);
	for (int i = 0; i < tzh_charcnt; i++) {

	}

	for (int i = 0; i < tzh_ttisutcnt; i++) {
		ttInfoTable[i].tt_isut = readBEu8(TZfile);
	}
	for (int i = 0; i < tzh_ttisstdcnt; i++) {
		ttInfoTable[i].tt_isstd = readBEu8(TZfile);
	}



	tzsetUTC();

	return 0;
}

void tzset(void)
{
	int res = tzset_internal();
	if (res == -1) {
		tzsetUTC();
	}
}
