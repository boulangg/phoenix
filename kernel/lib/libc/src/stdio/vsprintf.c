#include <stddef.h>
#include <ctype.h>
#include <stdarg.h>

// A format specifier follow the following prototype:
// %[flags][width][.precision][length]specifier

// Supported flags options
enum ARG_FLAG {
	FLAG_NONE,		// "(none)"
	FLAG_MINUS,		// "-"
	FLAG_PLUS,		// "+"
	FLAG_SPACE,		// "(space)"
	FLAG_HASH,		// "#"
	FLAG_ZERO		// "0"
};

// Supported length options
enum ARG_LENGTH {
	LENGTH_NONE,	// "(none)"
	LENGTH_HH,		// "hh"
	LENGTH_H,		// "h"
	LENGTH_L,		// "l"
	LENGTH_LL,		// "ll"
	LENGTH_J,		// "j"
	LENGTH_Z,		// "z"
	LENGTH_T,		// "t"
	LENGTH_L_MAJ	// "L"
};

// Supported specifiers
enum ARG_SPEC {
	SPEC_INT_DEC,			// "d" or "i"
	SPEC_UINT_DEC,			// "u"
	SPEC_UINT_OCT,			// "o"
	SPEC_UINT_HEX,			// "x"
	SPEC_UINT_HEX_MAJ,		// "X"
	SPEC_FLOAT_LOW,			// "f"
	SPEC_FLOAT_UPP,			// "F"
	SPEC_FLOAT_SCIEN,		// "e"
	SPEC_FLOAT_SCIEN_MAJ,	// "E"
	SPEC_FLOAT_SHORT,		// "g"
	SPEC_FLOAT_SHORT_MAJ,	// "G"
	SPEC_FLOAT_HEX,			// "a"
	SPEC_FLOAT_HEX_MAJ,		// "A"
	SPEC_CHAR,				// "c"
	SPEC_STR,				// "s"
	SPEC_PTR,				// "p"
	SPEC_NOTHING,			// "n"
	SPEC_PERCENT,			// "%"
	SPEC_ERROR				// "(invalid)"
};

// Supported width options
// "(number)"
// "*"

// Supported precision options
// ".(number)"
// ".*"

struct arg_info {
	enum ARG_FLAG flags;
	int width;
	int precision;
	enum ARG_LENGTH length;
	enum ARG_SPEC specifier;
};

// Print error if unsupported or incorrect format specifier
void print_error(char** str, struct arg_info* arg_info, va_list arg, const char* start);

void print_int(char** str, struct arg_info* arg_info, va_list arg, const char* start)
{
	(void)start;
	long long int num = 0;
	switch (arg_info->length) {
	case LENGTH_NONE:
	case LENGTH_HH:
	case LENGTH_H:
		num = (long long int)va_arg(arg, int);
		break;
	case LENGTH_L:
		num = (long long int)va_arg(arg, long int);
		break;
	case LENGTH_LL:
		num = (long long int)va_arg(arg, long long int);
		break;
	default:
		print_error(str, arg_info, arg, start);
		return;
	}
	int len = 0, n = num;

	if (num < 0) {
		*((*str)++) = '-';
		num = -num;
	}

	while (n != 0) {
		len++;
		n /= 10;
	}

	int rem;
	for (int i = 1; i <= len; i++) {
		rem = num % 10;
		num = num / 10;
		(*str)[len - i] = rem +'0';
	}
	(*str)+=len;
	(void)arg_info;
}

void print_uint(char** str, struct arg_info* arg_info, va_list arg, const char* start)
{
	(void)start;
	unsigned long long int num = 0;
	switch (arg_info->length) {
	case LENGTH_NONE:
	case LENGTH_HH:
	case LENGTH_H:
		num = (unsigned long long int)va_arg(arg, unsigned int);
		break;
	case LENGTH_L:
		num = (unsigned long long int)va_arg(arg, unsigned long int);
		break;
	case LENGTH_LL:
		num = (unsigned long long int)va_arg(arg, unsigned long long int);
		break;
	default:
		print_error(str, arg_info, arg, start);
		return;
	}
	int len = 0, n = num;
	int base = 10;
	char hex_offset = 'a'-10;
	switch (arg_info->specifier) {
	case SPEC_UINT_DEC:
		base = 10;
		break;
	case SPEC_UINT_HEX:
		base = 16;
		hex_offset = 'a'-10;
		break;
	case SPEC_UINT_HEX_MAJ:
		base = 16;
		hex_offset = 'A'-10;
		break;
	case SPEC_UINT_OCT:
		base = 8;
		break;
	default:
		print_error(str, arg_info, arg, start);
		return;
	}

	while (n != 0) {
		len++;
		n /= base;
	}

	int rem;
	for (int i = 1; i <= len; i++) {
		rem = num % base;
		num = num / base;
		if (rem < 10) {
			(*str)[len - i] = rem + '0';
		} else {
			(*str)[len - i] = rem + hex_offset;
		}
	}
	(*str)+=len;
	(void)arg_info;
}

void print_float(char** str, struct arg_info* arg_info, va_list arg, const char* start)
{
	(void)start;
	double flt = va_arg(arg, double);
	print_error(str, arg_info, arg, start);
	(void)flt;
	(void)str;
	(void)arg_info;
}

void print_char(char** str, struct arg_info* arg_info, va_list arg, const char* start)
{
	(void)start;
	*((*str)++) = (char)va_arg(arg, int);
	(void)arg_info;
}

void print_str(char** str, struct arg_info* arg_info, va_list arg, const char* start)
{
	(void)start;
	char *str_arg = va_arg(arg, char *);
	while (*str_arg != '\0') {
		*((*str)++) = *(str_arg++);
	}
	(void)arg_info;
}

void print_ptr(char** str, struct arg_info* arg_info, va_list arg, const char* start)
{
	(void)start;
	print_error(str, arg_info, arg, start);
	(void)str;
	(void)arg_info;
	(void)arg;
}

void print_nothing(char** str, struct arg_info* arg_info, va_list arg, const char* start)
{
	signed int* nb = va_arg(arg, signed int*);
	if (nb != NULL) {
		*nb = (*str) - start;
	}
	(void)str;
	(void)arg_info;
}

void print_percent(char** str, struct arg_info* arg_info, va_list arg, const char* start)
{
	(void)start;
	*((*str)++) = '%';
	(void)arg_info;
	(void)arg;
}

void print_error(char** str, struct arg_info* arg_info, va_list arg, const char* start)
{
	(void)start;
	*((*str)++) = 'E';
	*((*str)++) = 'R';
	*((*str)++) = 'R';
	*((*str)++) = 'O';
	*((*str)++) = 'R';
	*((*str)++) = ' ';
	(void)arg_info;
	(void)arg;
}

// Contain sprint option depending on the specifier
void (*sprint_arg_table[])(char** str, struct arg_info* arg_info, va_list arg, const char* start) = {
	print_int, 			print_uint, 		print_uint,
	print_uint, 		print_uint, 		print_float,
	print_float, 		print_float, 		print_float,
	print_float, 		print_float, 		print_float,
	print_float,		print_char, 		print_str,
	print_ptr,			print_nothing, 		print_percent,
	print_error
};

// Identify "flag" option, if any
void get_arg_flag(const char** format, struct arg_info* arg_info)
{
	switch(**format) {
	case '-':
		++(*format);
		arg_info->flags = FLAG_MINUS;
		break;
	case '+':
		++(*format);
		arg_info->flags = FLAG_PLUS;
		break;
	case ' ':
		++(*format);
		arg_info->flags = FLAG_SPACE;
		break;
	case '#':
		++(*format);
		arg_info->flags = FLAG_HASH;
		break;
	case '0':
		++(*format);
		arg_info->flags = FLAG_ZERO;
		break;
	default:
		//++(*format);
		arg_info->flags = FLAG_NONE;
		return;
	}
	return;
}

int read_int(const char** format)
{
	long int val = 0L;
	long int curr;
	while (isdigit(**format)) {
		curr = **format - '0';
		val *= 10;
		val += curr;
		++(*format);
	}
	return 0;
}

void get_arg_number(const char** format, int* number, va_list arg)
{
	switch(**format) {
	case '*':
		*number = va_arg(arg, int);
		++(*format);
		return;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		*number = read_int(format);
		return;
	default:
		*number = 0;
		return;
	}
}

// Identify "width" option, if any
void get_arg_width(const char** format, struct arg_info* arg_info, va_list arg)
{
	get_arg_number(format, &(arg_info->width), arg);
}

// Identify "precision" option, if any
void get_arg_precision(const char** format, struct arg_info* arg_info, va_list arg)
{
	switch(**format) {
	case '.':
		++(*format);
		get_arg_number(format, &(arg_info->precision), arg);
		return;
	default:
		arg_info->precision = 0;
		return;
	}
}

// Identify "length" option, if any
void get_arg_length(const char** format, struct arg_info* arg_info)
{
	switch(**format) {
	case 'h':
		++(*format);
		if (**format == 'h') {
			++(*format);
			arg_info->length = LENGTH_HH;
		} else {
			arg_info->length = LENGTH_H;
		}
		return;
	case 'l':
		++(*format);
		if (**format == 'l') {
			++(*format);
			arg_info->length = LENGTH_LL;
		} else {
			arg_info->length = LENGTH_L;
		}
		return;
	case 'j':
		++(*format);
		arg_info->length = LENGTH_J;
		return;
	case 'z':
		++(*format);
		arg_info->length = LENGTH_Z;
		return;
	case 't':
		++(*format);
		arg_info->length = LENGTH_T;
		return;
	case 'L':
		++(*format);
		arg_info->length = LENGTH_L_MAJ;
		return;
	default:
		arg_info->length = LENGTH_NONE;
		return;
	}
}

// Identify "specifier"
void get_arg_spec(const char** format, struct arg_info* arg_info)
{
	switch(**format) {
	case 'd':
		arg_info->specifier = SPEC_INT_DEC;
		break;
	case 'i':
		arg_info->specifier = SPEC_INT_DEC;
		break;
	case 'u':
		arg_info->specifier = SPEC_UINT_DEC;
		break;
	case 'o':
		arg_info->specifier = SPEC_UINT_OCT;
		break;
	case 'x':
		arg_info->specifier = SPEC_UINT_HEX;
		break;
	case 'X':
		arg_info->specifier = SPEC_UINT_HEX_MAJ;
		break;
	case 'f':
		arg_info->specifier = SPEC_FLOAT_HEX;
		break;
	case 'F':
		arg_info->specifier = SPEC_FLOAT_HEX_MAJ;
		break;
	case 'e':
		arg_info->specifier = SPEC_FLOAT_SCIEN;
		break;
	case 'E':
		arg_info->specifier = SPEC_FLOAT_SCIEN_MAJ;
		break;
	case 'g':
		arg_info->specifier = SPEC_FLOAT_SHORT;
		break;
	case 'G':
		arg_info->specifier = SPEC_FLOAT_SHORT_MAJ;
		break;
	case 'a':
		arg_info->specifier = SPEC_FLOAT_LOW;
		break;
	case 'A':
		arg_info->specifier = SPEC_FLOAT_UPP;
		break;
	case 'c':
		arg_info->specifier = SPEC_CHAR;
		break;
	case 's':
		arg_info->specifier = SPEC_STR;
		break;
	case 'p':
		arg_info->specifier = SPEC_PTR;
		break;
	case 'n':
		arg_info->specifier = SPEC_NOTHING;
		break;
	case '%':
		arg_info->specifier = SPEC_PERCENT;
		break;
	default:
		arg_info->specifier = SPEC_ERROR;
		return;
	}
	++(*format);
}

int vsprintf(char* str, const char* format, va_list arg)
{
	const char* format_start = format;
	struct arg_info arg_info;
	while (*format != '\0') {
		if (*format == '%') {
			++format;
			get_arg_flag(&format, &arg_info);
			get_arg_width(&format, &arg_info, arg);
			get_arg_precision(&format, &arg_info, arg);
			get_arg_length(&format, &arg_info);
			get_arg_spec(&format, &arg_info);
			sprint_arg_table[arg_info.specifier](&str, &arg_info, arg, format_start);
		} else {
			*str = *format;
			str++;
			format++;
		}
	}
	*str = '\0';
	return 0;
}
