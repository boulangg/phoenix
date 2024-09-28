/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include "bits/wordsize.h"

// General
#define __STDC_VERSION_STDDEF_H__ 202311L
typedef signed long ptrdiff_t;

#include "def/def_size.h"

#if __WORDSIZE == 64
typedef unsigned long max_align_t;
#else
typedef unsigned long long max_align_t;
#endif

#include "def/def_nullptr_t.h"
#include "def/def_null.h"
#include "def/def_unreachable.h"

#define offsetof(type, member) __builtin_offsetof(type, member)

#ifndef __cplusplus
typedef __WCHAR_TYPE__ wchar_t;
typedef __WINT_TYPE__ wint_t;
#endif
