/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 /******************************************************************************
 *
 * Name: acphoenix.h - OS specific defines, etc. for Phoenix
 *
 *****************************************************************************/

#ifndef __ACPHOENIX_H__
#define __ACPHOENIX_H__

#define ACPI_USE_STANDARD_HEADERS
#define ACPI_USE_SYSTEM_CLIBRARY

#if defined(__ia64__) || (defined(__x86_64__) && !defined(__ILP32__)) || defined(__aarch64__) || defined(__PPC64__) || \
    defined(__s390x__) || defined(__loongarch__) || (defined(__riscv) && (defined(__LP64__) || defined(_LP64)))
#define ACPI_MACHINE_WIDTH        64
#define COMPILER_DEPENDENT_INT64  long
#define COMPILER_DEPENDENT_UINT64 unsigned long
#else
#define ACPI_MACHINE_WIDTH        32
#define COMPILER_DEPENDENT_INT64  long long
#define COMPILER_DEPENDENT_UINT64 unsigned long long
#define ACPI_USE_NATIVE_DIVIDE
#define ACPI_USE_NATIVE_MATH64
#endif

#undef ACPI_DEBUG_OUTPUT

#define ACPI_CACHE_T         ACPI_MEMORY_LIST
#define ACPI_USE_LOCAL_CACHE 1

#endif // __ACPHOENIX_H__
