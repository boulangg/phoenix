#ifndef _MM_SBRK_H_
#define _MM_SBRK_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void *sbrk(ptrdiff_t increment);

#ifdef __cplusplus
}
#endif

#endif // _MM_SBRK_H_
