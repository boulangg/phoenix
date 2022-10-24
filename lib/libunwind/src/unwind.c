#include <stdint.h>

#include <stdio.h>
#include <stdlib.h>
#include <unwind.h>

#include "debug.h"
#include "exception_frame.h"
#include "unwind_internal.h"

// Throwing an Exception

_Unwind_Reason_Code _Unwind_Phase2(struct _Unwind_Exception* exception_object, struct _Unwind_Context* context)
{
	_Unwind_Action actions;
	__personality_routine personality = 0;

	actions = _UA_CLEANUP_PHASE;
	while (1) {
		int64_t ret = unwind_next_step(context);
		if (ret == 0) {
			//return _URC_END_OF_STACK;
		} else if (ret < 0) {
			return _URC_FATAL_PHASE1_ERROR;
		}

		personality = (__personality_routine)(uintptr_t)context->personality;
		if (personality) {
			_Unwind_Reason_Code reasonCode = (*personality)(
				_Unwind_Version, actions,
				*(uint64_t*)&(exception_object->exception_class), exception_object,
				context);
			if (reasonCode != _URC_CONTINUE_UNWIND) {
				if (reasonCode == _URC_INSTALL_CONTEXT) {
					setContext(&context->cursor);
					abort();
				} else {
					return _URC_FATAL_PHASE2_ERROR;
				}
			}
		}
	}
}

_Unwind_Reason_Code _Unwind_RaiseException(struct _Unwind_Exception* exception_object)
{
	struct _Unwind_Context context = {};
	__personality_routine personality = 0;
	_Unwind_Action actions;

	getContext(&context.cursor);

	// Unwind Phase 1
	actions = _UA_SEARCH_PHASE;
	while (1) {
		int ret = unwind_next_step(&context);
		if (ret == 0) {
			return _URC_END_OF_STACK;
		} else if (ret < 0) {
			return _URC_FATAL_PHASE1_ERROR;
		}

		personality = (__personality_routine)(uintptr_t)context.personality;
		if (personality) {
			_Unwind_Reason_Code reasonCode = (*personality)(
				_Unwind_Version, actions,
				exception_object->exception_class, exception_object,
				&context);
			if (reasonCode != _URC_CONTINUE_UNWIND) {
				if (reasonCode == _URC_HANDLER_FOUND) {
					break;
				} else {
					return _URC_FATAL_PHASE1_ERROR;
				}
			}
		}
	}

	uint64_t stop_frame_marker = _Unwind_GetIP(&context);
	(void)stop_frame_marker;

	// Reset cursor
	getContext(&context.cursor);

	// Unwind Phase 2
	return _Unwind_Phase2(exception_object, &context);
}

//_Unwind_Reason_Code _Unwind_ForcedUnwind(
//	struct _Unwind_Exception* exception_object,
//	_Unwind_Stop_Fn stop,
//	void* stop_parameter)
//{
//	struct _Unwind_Context context = {};
//
//	// TODO use stop/stop_parameter instead of personality
//
//	// Get cursor
//	getContext(&context.cursor);
//
//	// Unwind Phase 2
//	return _Unwind_Phase2(exception_object, &context);
//}

void _Unwind_Resume(struct _Unwind_Exception* exception_object)
{
	struct _Unwind_Context context = {};

	// Get cursor
	getContext(&context.cursor);

	// Unwind Phase 2
	_Unwind_Phase2(exception_object, &context);
}

// Exception Object Management
//void _Unwind_DeleteException(struct _Unwind_Exception* exception_object)
//{
//	_Unwind_Exception_Cleanup_Fn cleanup = exception_object->exception_cleanup;
//
//	if (cleanup) {
//		(*cleanup)(_URC_FOREIGN_EXCEPTION_CAUGHT, exception_object);
//	}
//}

// Context Management
uint64_t _Unwind_GetGR(struct _Unwind_Context* context, int index)
{
	return context->cursor.regs[index];
}

void _Unwind_SetGR(
	struct _Unwind_Context* context,
	int index,
	uint64_t new_value)
{
	context->cursor.regs[index] = new_value;
}

uint64_t _Unwind_GetIP(struct _Unwind_Context* context)
{
	return context->cursor.regs[DW_X86_64_RIP];
}

void _Unwind_SetIP(struct _Unwind_Context* context, uint64_t new_value)
{
	context->cursor.regs[DW_X86_64_RIP] = new_value;
}

uint64_t _Unwind_GetLanguageSpecificData(struct _Unwind_Context* context)
{
	return context->lsda;
}

uint64_t _Unwind_GetRegionStart(struct _Unwind_Context* context)
{
	return context->ipStart;
}
