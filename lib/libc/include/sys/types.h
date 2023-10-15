/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef __TYPES_H__
#define __TYPES_H__

// Time types
typedef unsigned long long clock_t;
typedef unsigned int clockid_t;
typedef signed long long time_t;
typedef unsigned long long timer_t;
typedef signed int suseconds_t;
typedef unsigned int useconds_t;

// Device types
typedef unsigned long dev_t;

// Filesystem and filesystem types
typedef signed long long blkcnt_t;
typedef signed int blksize_t;
typedef unsigned long long fsblkcnt_t;
typedef unsigned long long fsfilcnt_t;
typedef unsigned long ino_t;
typedef unsigned int mode_t;
typedef unsigned long nlink_t;
typedef signed long off_t;
typedef signed long long loff_t;

// IPC types
typedef unsigned long long key_t;

// Size types
#include <def/def_size.h>
typedef signed long ssize_t;

// Id types
typedef unsigned int id_t;
typedef id_t gid_t;
typedef id_t pid_t;
typedef id_t uid_t;

// pthread types
// typedef unsigned long long pthread_attr_t;
// typedef unsigned long long pthread_barrier_t;
// typedef unsigned long long pthread_barrierattr_t;
// typedef unsigned long long pthread_cond_t;
// typedef unsigned long long pthread_condattr_t;
// typedef unsigned long long pthread_key_t;
// typedef unsigned long long pthread_mutex_t;
// typedef unsigned long long pthread_mutexattr_t;
// typedef unsigned long long pthread_once_t;
// typedef unsigned long long pthread_rwlock_t;
// typedef unsigned long long pthread_rwlockattr_t;
// typedef unsigned long long pthread_spinlock_t;
// typedef unsigned long long pthread_t;

#endif // __TYPES_H__
