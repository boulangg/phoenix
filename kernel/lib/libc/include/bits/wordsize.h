#ifndef _KERNEL_BITS_WORDSIZE_H_
#define _KERNEL_BITS_WORDSIZE_H_

#if defined(__LP64__)
#define __WORDSIZE 64
#else
#define __WORDSIZE 32
#endif // __LP64__

#endif // _KERNEL_BITS_WORDSIZE_H_
