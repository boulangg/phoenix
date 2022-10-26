#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int main(int argc, char* argv[])
{
	char* filename = "/etc/localtime";
	if (argc > 1) {
		filename = argv[1];
	}

	FILE* TZfile = fopen(filename, "rb");

	if (TZfile == NULL) {
		return -1;
	}

	// Verify tzfile
	char TZfileStart[20];
	fread(TZfileStart, 1, 20, TZfile);
	if (strncmp(TZfileStart, "TZif", 4) != 0) {
		// Not a TZfile
		printf("Not a valid TZfile");
		return -1;
	}
	if (!(TZfileStart[4] == '\0' || TZfileStart[4] == '2' || TZfileStart[4] == '3')) {
		// Format not recognized
		printf("Format not recognized");
		return -1;
	}

	uint32_t tzh_ttisutcnt = readBEu32(TZfile);		// The number of UT/local indicators stored in the file.
	uint32_t tzh_ttisstdcnt = readBEu32(TZfile);	// The number of standard/wall indicators stored in the file.
	uint32_t tzh_leapcnt = readBEu32(TZfile);		// The number of leap seconds for which data entries are stored in the file.
	uint32_t tzh_timecnt = readBEu32(TZfile);		// The number of transition times for which data entries are stored in the file.
	uint32_t tzh_typecnt = readBEu32(TZfile);		// The number of local time types for which data entries are stored in the file(must not be zero).
	uint32_t tzh_charcnt = readBEu32(TZfile);		// The number of bytes of time zone abbreviation strings stored in the file.

	struct tt* ttTable = (struct tt*)calloc(tzh_timecnt, sizeof(struct tt));
	for (uint32_t i = 0; i < tzh_timecnt; i++) {
		ttTable[i].tt_time = readBEs32(TZfile);
	}
	for (uint32_t i = 0; i < tzh_timecnt; i++) {
		ttTable[i].tt_type = readBEu8(TZfile);
	}

	struct ttinfo* ttInfoTable = (struct ttinfo*)calloc(tzh_typecnt, sizeof(struct ttinfo));
	for (uint32_t i = 0; i < tzh_typecnt; i++) {
		ttInfoTable[i].tt_utoff = readBEs32(TZfile);
		ttInfoTable[i].tt_isdst = readBEu8(TZfile);
		ttInfoTable[i].tt_desigidx = readBEu8(TZfile);
	}

	struct ttleap* ttLeapTable = (struct ttleap*)calloc(tzh_leapcnt, sizeof(struct ttleap));
	for (uint32_t i = 0; i < tzh_leapcnt; i++) {
		ttLeapTable[i].tt_start = readBEu32(TZfile);
		ttLeapTable[i].tt_leap = readBEs32(TZfile);
	}

	char* ttStrTable = (char*)malloc(tzh_charcnt);
	fread(ttStrTable, tzh_charcnt, 1, TZfile);

	for (uint32_t i = 0; i < tzh_ttisutcnt; i++) {
		ttInfoTable[i].tt_isut = readBEu8(TZfile);
	}
	for (uint32_t i = 0; i < tzh_ttisstdcnt; i++) {
		ttInfoTable[i].tt_isstd = readBEu8(TZfile);
	}

	for (uint32_t i = 0; i < tzh_timecnt; i++) {
		uint8_t type = ttTable[i].tt_type;
		uint8_t desigidx = ttInfoTable[type].tt_desigidx;
		printf("%08x, %-4s, isdst=%i utoff=%i\n", ttTable[i].tt_time, ttStrTable + desigidx, ttInfoTable[type].tt_isdst, ttInfoTable[type].tt_utoff);
	}

	free(ttTable);
	free(ttInfoTable);
	free(ttLeapTable);
	free(ttStrTable);

	return 0;
}
