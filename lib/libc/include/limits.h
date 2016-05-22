#ifndef _LIMITS_H_
#define _LIMITS_H_

// 7.10 Sizes of integer types <limits.h>
#include "bits/wordsize.h"

// 5.2.4.2.1 Sizes of integer types <limits.h>
#define CHAR_BIT	8
#define SCHAR_MIN	(-128)
#define SCHAR_MAX	+127
#define UCHAR_MAX	255

#ifdef __CHAR_UNSIGNED__
#define CHAR_MIN	0
#define CHAR_MAX	UCHAR_MAX
#else
#define CHAR_MIN	SCHAR_MIN
#define CHAR_MAX	SCHAR_MAX
#endif	// __CHAR_UNSIGNED__

#define MB_LEN_MAX	1

#define SHRT_MIN	(-32768)
#define SHRT_MAX 	+32767
#define USHRT_MAX	65535

#define INT_MIN		-2147483648
#define INT_MAX		2147483647
#define UINT_MAX	4294967295U

#define LLONG_MIN	-9223372036854775807LL
#define LLONG_MAX	9223372036854775807LL
#define ULLONG_MAX	18446744073709551615ULL

#if __WORDSIZE == 64
#define LONG_MIN	LLONG_MIN
#define LONG_MAX	LLONG_MAX
#define ULONG_MAX	ULLONG_MAX
#else
#define LONG_MIN	INT_MIN
#define LONG_MAX	INT_MAX
#define ULONG_MAX	UINT_MAX
#endif // __WORDSIZE == 64


#endif // _LIMITS_H_
