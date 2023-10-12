/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "io.h"

#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

// A format specifier follow the following prototype:
// %[flags][width][.precision][length]specifier

// Supported flags options
enum ARG_FLAG
{
    FLAG_NONE = 0x00,  // "(none)"
    FLAG_MINUS = 0x01, // "-"
    FLAG_PLUS = 0x02,  // "+"
    FLAG_SPACE = 0x04, // "(space)"
    FLAG_HASH = 0x08,  // "#"
    FLAG_ZERO = 0x10   // "0"
};

// Supported length options
enum ARG_LENGTH
{
    LENGTH_NONE, // "(none)"
    LENGTH_HH,   // "hh"
    LENGTH_H,    // "h"
    LENGTH_L,    // "l"
    LENGTH_LL,   // "ll"
    LENGTH_J,    // "j"
    LENGTH_Z,    // "z"
    LENGTH_T,    // "t"
    LENGTH_L_MAJ // "L"
};

// Supported specifiers
enum ARG_SPEC
{
    SPEC_INT_DEC,         // "d" or "i"
    SPEC_UINT_DEC,        // "u"
    SPEC_UINT_OCT,        // "o"
    SPEC_UINT_HEX,        // "x"
    SPEC_UINT_HEX_MAJ,    // "X"
    SPEC_FLOAT_LOW,       // "f"
    SPEC_FLOAT_UPP,       // "F"
    SPEC_FLOAT_SCIEN,     // "e"
    SPEC_FLOAT_SCIEN_MAJ, // "E"
    SPEC_FLOAT_SHORT,     // "g"
    SPEC_FLOAT_SHORT_MAJ, // "G"
    SPEC_FLOAT_HEX,       // "a"
    SPEC_FLOAT_HEX_MAJ,   // "A"
    SPEC_CHAR,            // "c"
    SPEC_STR,             // "s"
    SPEC_PTR,             // "p"
    SPEC_NOTHING,         // "n"
    SPEC_PERCENT,         // "%"
    SPEC_ERROR            // "(invalid)"
};

// Supported width options
// "(number)"
// "*"

// Supported precision options
// ".(number)"
// ".*"

struct arg_info
{
    int flags;
    int width;
    int precision;
    enum ARG_LENGTH length;
    enum ARG_SPEC specifier;
};

static char intToHexMaj[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
static char intToHexMin[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

// Print error if unsupported or incorrect format specifier
static int print_error(FILE* str, struct arg_info* arg_info, va_list arg, int* count)
{
    (void)arg_info;
    (void)arg;

    fputs("ERROR ", str);
    (*count) += 6;
    return 0;
}

static int print_int(FILE* str, struct arg_info* arg_info, va_list arg, int* count)
{
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
        return print_error(str, arg_info, arg, count);
    }

    long long int len = 0, n = num;
    int BASE = 10;
    char buf[20]; // enough space for max long long int
    char sign = 0;

    if (num >= 0) {
        if (arg_info->flags & FLAG_PLUS) {
            sign = '+';
        } else if (arg_info->flags & FLAG_SPACE) {
            sign = ' ';
        }
    } else {
        sign = '-';
        num = -num;
    }

    while (n != 0) {
        len++;
        n /= BASE;
    }

    if (arg_info->precision == -1) {
        if (arg_info->flags & FLAG_ZERO) {
            arg_info->precision = abs(arg_info->width);
            if (sign) {
                arg_info->precision -= 1;
            }
            if (arg_info->precision <= 0) {
                arg_info->precision = 1;
            }
        } else {
            arg_info->precision = 1;
        }
    }

    int nbZero = 0;
    if (len < arg_info->precision) {
        nbZero = arg_info->precision - len;
    }

    if (sign) {
        len++;
    }

    int nbSpace = 0;
    if (len + nbZero < abs(arg_info->width)) {
        nbSpace = abs(arg_info->width) - (len + nbZero);
    }

    // Right-justify
    if (!(arg_info->flags & FLAG_MINUS)) {
        for (int i = 0; i < nbSpace; i++) {
            fputc(' ', str);
        }
    }

    if (sign) {
        fputc(sign, str);
        *count += 1;
        len--;
    }

    for (int i = 0; i < nbZero; i++) {
        fputc('0', str);
    }

    unsigned long long rem;
    for (int i = 0; i < len; i++) {
        rem = num % BASE;
        num = num / BASE;
        buf[len - 1 - i] = intToHexMaj[rem];
    }
    for (int i = 0; i < len; i++) {
        fputc(buf[i], str);
    }

    // Left-justify
    if (arg_info->flags & FLAG_MINUS) {
        for (int i = 0; i < nbSpace; i++) {
            fputc(' ', str);
        }
    }

    *count += nbSpace + nbZero + len;
    return 0;
}

static int print_uint(FILE* str, struct arg_info* arg_info, va_list arg, int* count)
{
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
        return print_error(str, arg_info, arg, count);
    }

    int len = 0;
    unsigned long long n = num;
    int base = 10;
    char buf[23]; // enough space for max unsigned long long in octal
    char* intToHex = intToHexMin;
    char hash = 0;
    switch (arg_info->specifier) {
    case SPEC_UINT_DEC:
        base = 10;
        break;
    case SPEC_UINT_HEX:
        base = 16;
        intToHex = intToHexMin;
        hash = 'x';
        break;
    case SPEC_UINT_HEX_MAJ:
        intToHex = intToHexMaj;
        base = 16;
        hash = 'X';
        break;
    case SPEC_UINT_OCT:
        base = 8;
        break;
    default:
        return print_error(str, arg_info, arg, count);
    }

    if (arg_info->precision == -1) {
        if (arg_info->flags & FLAG_ZERO) {
            arg_info->precision = arg_info->width;
            if (arg_info->flags & FLAG_HASH) {
                arg_info->precision -= 1;
                if (hash) {
                    arg_info->precision -= 1;
                }
            }
        }
        if (arg_info->precision <= 0) {
            arg_info->precision = 1;
        }
    }

    while (n != 0) {
        len++;
        n /= base;
    }

    // Count leading 0
    int nbZero = 0;
    if (len < arg_info->precision) {
        nbZero = arg_info->precision - len;
    }

    // Increase len with HASH
    if ((arg_info->flags & FLAG_HASH) && (arg_info->specifier != SPEC_UINT_DEC)) {
        len++;
        if (hash) {
            len++;
        }
    }

    // Count space
    int nbSpace = 0;
    if (len + nbZero < arg_info->width) {
        nbSpace = arg_info->width - (len + nbZero);
    }

    // Right-justify
    if (!(arg_info->flags & FLAG_MINUS)) {
        for (int i = 0; i < nbSpace; i++) {
            fputc(' ', str);
        }
    }

    // Print HASH
    if ((arg_info->flags & FLAG_HASH) && (arg_info->specifier != SPEC_UINT_DEC)) {
        len--;
        fputc('0', str);
        *count += 1;
        if (hash) {
            fputc(hash, str);
            *count += 1;
            len--;
        }
    }

    // Print leading 0
    for (int i = 0; i < nbZero; i++) {
        fputc('0', str);
    }

    // Print number
    unsigned long long rem;
    for (int i = 0; i < len; i++) {
        rem = num % base;
        num = num / base;
        buf[len - 1 - i] = intToHex[rem];
    }
    for (int i = 0; i < len; i++) {
        fputc(buf[i], str);
    }

    // Left-justify
    if (arg_info->flags & FLAG_MINUS) {
        for (int i = 0; i < nbSpace; i++) {
            fputc(' ', str);
        }
    }

    *(count) += nbSpace + nbZero + len;
    return 0;
}

static int print_float(FILE* str, struct arg_info* arg_info, va_list arg, int* count)
{
    // TODO implement printf float
    double flt = va_arg(arg, double);
    (void)flt;
    return print_error(str, arg_info, arg, count);
}

static int print_char(FILE* str, struct arg_info* arg_info, va_list arg, int* count)
{
    (void)arg_info;

    char c = (char)va_arg(arg, int);
    fputc(c, str);
    (*count)++;
    return 0;
}

static int print_str(FILE* str, struct arg_info* arg_info, va_list arg, int* count)
{
    (void)arg_info;

    char* str_arg = va_arg(arg, char*);

    int len = strlen(str_arg);

    // Count space
    int nbSpace = 0;
    if (len < arg_info->width) {
        nbSpace = arg_info->width - (len);
    }

    // Right-justify
    if (!(arg_info->flags & FLAG_MINUS)) {
        for (int i = 0; i < nbSpace; i++) {
            fputc(' ', str);
        }
    }

    while (*str_arg != '\0') {
        fputc(*str_arg, str);
        str_arg++;
    }

    // Left-justify
    if (arg_info->flags & FLAG_MINUS) {
        for (int i = 0; i < nbSpace; i++) {
            fputc(' ', str);
        }
    }
    (*count) += nbSpace + len;

    return 0;
}

static int print_ptr(FILE* str, struct arg_info* arg_info, va_list arg, int* count)
{
    arg_info->specifier = SPEC_UINT_HEX;
    arg_info->precision = 16;
    arg_info->length = LENGTH_LL;
    return print_uint(str, arg_info, arg, count);
}

static int print_nothing(FILE* str, struct arg_info* arg_info, va_list arg, int* count)
{
    (void)str;
    (void)arg_info;

    signed int* nb = va_arg(arg, signed int*);
    if (nb != NULL) {
        *nb = *count;
    }

    return 0;
}

static int print_percent(FILE* str, struct arg_info* arg_info, va_list arg, int* count)
{
    (void)arg_info;
    (void)arg;

    fputc('%', str);
    (*count)++;
    return 0;
}

// Contain sprint option depending on the specifier
int (*fprintf_arg_table[])(FILE* str, struct arg_info* arg_info, va_list arg, int* count) = {
    print_int,   print_uint,  print_uint,    print_uint,    print_uint,  print_float, print_float,
    print_float, print_float, print_float,   print_float,   print_float, print_float, print_char,
    print_str,   print_ptr,   print_nothing, print_percent, print_error};

// Identify "flag" option, if any
static void get_arg_flag(const char** format, struct arg_info* arg_info)
{
    arg_info->flags = FLAG_NONE;
    while (1) {
        switch (**format) {
        case '-':
            ++(*format);
            arg_info->flags |= FLAG_MINUS;
            break;
        case '+':
            ++(*format);
            arg_info->flags |= FLAG_PLUS;
            break;
        case ' ':
            ++(*format);
            arg_info->flags |= FLAG_SPACE;
            break;
        case '#':
            ++(*format);
            arg_info->flags |= FLAG_HASH;
            break;
        case '0':
            ++(*format);
            arg_info->flags |= FLAG_ZERO;
            break;
        default:
            return;
        }
    }
    return;
}

static int read_int(const char** format)
{
    long int val = 0L;
    long int curr;
    while (isdigit(**format)) {
        curr = **format - '0';
        val *= 10;
        val += curr;
        ++(*format);
    }
    return val;
}

static void get_arg_number(const char** format, int* number, va_list arg)
{
    switch (**format) {
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
static void get_arg_width(const char** format, struct arg_info* arg_info, va_list arg)
{
    int width;
    get_arg_number(format, &(width), arg);
    if (width < 0) {
        arg_info->flags |= FLAG_MINUS;
    }
    arg_info->width = abs(width);
}

// Identify "precision" option, if any
static void get_arg_precision(const char** format, struct arg_info* arg_info, va_list arg)
{
    switch (**format) {
    case '.':
        ++(*format);
        get_arg_number(format, &(arg_info->precision), arg);
        if (arg_info->width < 0) {
            arg_info->width = 0;
        }
        return;
    default:
        arg_info->precision = -1; // Default
        return;
    }
}

// Identify "length" option, if any
static void get_arg_length(const char** format, struct arg_info* arg_info)
{
    switch (**format) {
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
static void get_arg_spec(const char** format, struct arg_info* arg_info)
{
    switch (**format) {
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

int vfprintf(FILE* str, const char* format, va_list arg)
{
    CHECK_FILE(str);

    int arg_count = 0;

    struct arg_info arg_info;
    while (*format != '\0') {
        if (*format == '%') {
            ++format;
            get_arg_flag(&format, &arg_info);
            get_arg_width(&format, &arg_info, arg);
            get_arg_precision(&format, &arg_info, arg);
            get_arg_length(&format, &arg_info);
            get_arg_spec(&format, &arg_info);
            int ret = fprintf_arg_table[arg_info.specifier](str, &arg_info, arg, &arg_count);
            if (ret < 0) {
                return ret;
            }
        } else {
            fputc(*format, str);
            arg_count++;
            format++;
        }
    }
    return arg_count;
}
