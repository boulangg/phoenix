/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef _TERMIOS_H_
#define _TERMIOS_H_

#include <stddef.h>
#include <stdint.h>

// Control characters name
#define VEOF   0
#define VEOL   1
#define VERASE 2
#define VINTR  3
#define VKILL  4
#define VMIN   6
#define VQUIT  7
#define VSTART 8
#define VSTOP  9
#define VSUSP  10
#define VTIME  11
#define NCCS   12

// Input modes
#define BRKINT 0x0001
#define ICRNL  0x0002
#define IGNBRK 0x0004
#define IGNCR  0x0008
#define IGNPAR 0x0010
#define INLCR  0x0020
#define INPCK  0x0040
#define ISTRIP 0x0080
#define IUCLC  0x0100
#define IXANY  0x0200
#define IXOFF  0x0400
#define PARMRK 0x0800

// Output modes
#define OPOST  0x0001
#define OLCUL  0x0002
#define ONLCR  0x0004
#define OCRNL  0x0008
#define ONOCR  0x0010
#define ONLRET 0x0020
#define OFILL  0x0040
#define OFDEL  0x0080
#define NLDLY  0x0100
#define NL0    0x0000
#define NL1    0x0100
#define CRDLY  0x0600
#define CR0    0x0000
#define CR1    0x0200
#define CR2    0x0400
#define CR3    0x0600
#define TABDLY 0x1800
#define TAB0   0x0000
#define TAB1   0x0800
#define TAB2   0x1000
#define TAB3   0x1C00
#define BSDLY  0x2000
#define BS0    0x0000
#define BS1    0x2000
#define VTDLY  0x4000
#define VT0    0x0000
#define VT1    0x4000
#define FFDLY  0x8000
#define FF0    0x0000
#define FF1    0x8000

// Baud Rate Selection
#define B0     0
#define B50    50
#define B75    75
#define B110   110
#define B134   134
#define B150   150
#define B200   200
#define B300   300
#define B600   600
#define B1200  1200
#define B1800  1800
#define B2400  2400
#define B4800  4800
#define B9600  9600
#define B19200 19200
#define B38400 38400

// Control modes
#define CLOCAL 0x0001
#define CREAD  0x0002
#define CSIZE  0x000C
#define CS5    0x0000
#define CS6    0x0004
#define CS7    0x0008
#define CS8    0x000C
#define CSTOP8 0x0010
#define HUPCL  0x0020
#define PARENB 0x0040
#define PARODD 0x0080

// Local modes
#define ECHO   0x0001
#define ECHOE  0x0002
#define ECHOK  0x0004
#define ECHONL 0x0008
#define ICANON 0x0010
#define IEXTEN 0x0020
#define ISIG   0x0040
#define NOFLSH 0x0080
#define TOSTOP 0x0100
#define XCASE  0x0200

typedef char cc_t;
typedef size_t speed_t;
typedef uint32_t tcflag_t;

struct termios
{
    tcflag_t c_iflag; // input modes
    tcflag_t c_oflag; // output modes
    tcflag_t c_cflag; // control modes
    tcflag_t c_lflag; // local modes
    cc_t c_cc[NCCS];  // control chars
    speed_t c_ispeed; // input speed
    speed_t c_ospeed; // output speed
};

#endif // _TERMIOS_H_
