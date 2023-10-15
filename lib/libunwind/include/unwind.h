/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

// Data Structures

typedef enum _Unwind_Reason_Code
{
    _URC_NO_REASON = 0,
    _URC_FOREIGN_EXCEPTION_CAUGHT = 1,
    _URC_FATAL_PHASE2_ERROR = 2,
    _URC_FATAL_PHASE1_ERROR = 3,
    _URC_NORMAL_STOP = 4,
    _URC_END_OF_STACK = 5,
    _URC_HANDLER_FOUND = 6,
    _URC_INSTALL_CONTEXT = 7,
    _URC_CONTINUE_UNWIND = 8
} _Unwind_Reason_Code;

struct _Unwind_Exception;
typedef void (*_Unwind_Exception_Cleanup_Fn)(_Unwind_Reason_Code reason, struct _Unwind_Exception* exc);

struct _Unwind_Exception
{
    uint64_t exception_class;
    _Unwind_Exception_Cleanup_Fn exception_cleanup;
    uint64_t private_1;
    uint64_t private_2;
};

struct _Unwind_Context;

// Throwing an Exception

_Unwind_Reason_Code _Unwind_RaiseException(struct _Unwind_Exception* exception_object);

typedef int _Unwind_Action;

typedef _Unwind_Reason_Code (*_Unwind_Stop_Fn)(int version, _Unwind_Action actions, uint64_t exceptionClass,
                                               struct _Unwind_Exception* exceptionObject,
                                               struct _Unwind_Context* context, void* stop_parameter);

_Unwind_Reason_Code _Unwind_ForcedUnwind(struct _Unwind_Exception* exception_object, _Unwind_Stop_Fn stop,
                                         void* stop_parameter);

void _Unwind_Resume(struct _Unwind_Exception* exception_object);

// Exception Object Management

void _Unwind_DeleteException(struct _Unwind_Exception* exception_object);

// Context Management

uint64_t _Unwind_GetGR(struct _Unwind_Context* context, int index);

void _Unwind_SetGR(struct _Unwind_Context* context, int index, uint64_t new_value);

uint64_t _Unwind_GetIP(struct _Unwind_Context* context);

void _Unwind_SetIP(struct _Unwind_Context* context, uint64_t new_value);

uint64_t _Unwind_GetLanguageSpecificData(struct _Unwind_Context* context);

uint64_t _Unwind_GetRegionStart(struct _Unwind_Context* context);

// Personality Routine

static const _Unwind_Action _UA_SEARCH_PHASE = 1;
static const _Unwind_Action _UA_CLEANUP_PHASE = 2;
static const _Unwind_Action _UA_HANDLER_FRAME = 4;
static const _Unwind_Action _UA_FORCE_UNWIND = 8;

typedef _Unwind_Reason_Code (*__personality_routine)(int version, _Unwind_Action actions, uint64_t exceptionClass,
                                                     struct _Unwind_Exception* exceptionObject,
                                                     struct _Unwind_Context* context);

#ifdef __cplusplus
}
#endif
