/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include <def/def_size.h>

#include <sys/types.h>

// Protection options
#define PROT_READ  0x1 /* Page can be read. */
#define PROT_WRITE 0x2 /* Page can be written. */
#define PROT_EXEC  0x4 /* Page can be executed. */
#define PROT_NONE  0x0 /* Page can not be accessed. */

// Flag options
#define MAP_SHARED    0x01     /* Share changes. */
#define MAP_PRIVATE   0x02     /* Changes are private. */
#define MAP_FIXED     0x10     /* Interpret addr exactly. */
#define MAP_ANON      0x1000   /* allocated from memory, swap space */
#define MAP_ANONYMOUS MAP_ANON /* alternate POSIX spelling */

// msync optinos
#define MS_ASYNC      1 /* Sync memory asynchronously. */
#define MS_INVALIDATE 2 /* Invalidate the caches. */
#define MS_SYNC       4 /* Synchronous memory sync. */

// mlockall options
#define MCL_CURRENT 1 /* Lock all currently mapped pages. */
#define MCL_FUTURE  2 /* Lock all additions to address space. */

#ifdef __cplusplus
extern "C" {
#endif

int mlock(const void* addr, size_t length);
int mlockall(int flags);
int munlock(const void* addr, size_t length);
int munlockall(void);

void* mmap(void* addr, size_t length, int prot, int flags, int fd, off_t offset);
int munmap(void* addr, size_t length);

int mprotect(void* addr, size_t length, int prot);
int msync(void* addr, size_t length, int flags);

int shm_open(const char* name, int oflag, mode_t mode);
int shm_unlink(const char* name);

#ifdef __cplusplus
}
#endif
