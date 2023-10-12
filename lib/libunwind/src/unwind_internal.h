/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include "exception_frame.h"
#include "unwind.h"

typedef struct _Unwind_Context
{
    unwind_cursor_t cursor;
    uint64_t personality;
    uint64_t lsda;
    uint64_t ipStart;
} _Unwind_Context_t;

#ifdef __cplusplus
extern "C" {
#endif

static const int _Unwind_Version = 1;

void getContext(unwind_cursor_t* cursor);
void setContext(unwind_cursor_t* cursor);

int64_t unwind_next_step(struct _Unwind_Context* cursor);

#ifdef __cplusplus
}
#endif
