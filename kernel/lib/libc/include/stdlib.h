#ifndef _KERNEL_STDLIB_H_
#define _KERNEL_STDLIB_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// Dynamic memory management
void* calloc(size_t num, size_t size);
void free(void* ptr);
void* malloc(size_t size);
void* realloc(void* ptr, size_t size);


#ifdef __cplusplus
}
#endif

#endif /* _KERNEL_STDLIB_H_ */
