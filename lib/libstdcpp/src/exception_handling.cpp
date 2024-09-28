/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <new>
#include <typeinfo>

#include <arch/x86_64/cpu_context.h>
#include <debug.h>
#include <dwarf_decode.h>
#include <stdlib.h>
#include <string.h>
#include <unwind.h>

struct __cxa_exception
{
    typedef void (*unexpected_handler)();
    typedef void (*terminate_handler)();

    std::type_info* exceptionType;
    void (*exceptionDestructor)(void*);
    unexpected_handler unexpectedHandler;
    terminate_handler terminateHandler;
    __cxa_exception* nextException;

    int handlerCount;
    int handlerSwitchValue;
    const char* actionRecord;
    const char* languageSpecificData;
    void* catchTemp;
    void* adjustedPtr;

    _Unwind_Exception unwindHeader;
};

struct __cxa_eh_globals
{
    __cxa_exception* caughtExceptions;
    unsigned int uncaughtExceptions;
};

static struct __cxa_eh_globals _currentThreadException = {NULL, 0};

__cxa_eh_globals* __cxa_get_globals(void)
{
    return &_currentThreadException;
}

__cxa_eh_globals* __cxa_get_globals_fast(void)
{
    return &_currentThreadException;
}

static __cxa_exception* cxa_exception_from_thrown_exception(void* thrown_exception)
{
    return static_cast<__cxa_exception*>(thrown_exception) - 1;
}

static __cxa_exception* cxa_exception_from_unwind_exception(_Unwind_Exception* unwind_exception)
{
    return cxa_exception_from_thrown_exception(unwind_exception + 1);
}

static _Unwind_Exception* unwind_exception_from_cxa_exception(__cxa_exception* cxa_exception)
{
    return &(cxa_exception->unwindHeader);
}

static void* thrown_exception_from_cxa_exception(__cxa_exception* cxa_exception)
{
    return static_cast<void*>(cxa_exception + 1);
}

extern "C"
{
void* __cxa_allocate_exception(std::size_t thrown_size)
{
    size_t exceptionSize = sizeof(__cxa_exception) + thrown_size;

    void* buffer = malloc(exceptionSize);
    __cxa_exception* exception_header = static_cast<__cxa_exception*>(buffer);
    memset(exception_header, 0, exceptionSize);

    return thrown_exception_from_cxa_exception(exception_header);
}

void __cxa_free_exception(__cxa_exception* thrown_exception)
{
    free(thrown_exception);
}

void __cxa_exception_cleanup(_Unwind_Reason_Code code, _Unwind_Exception* exc)
{
    DEBUG_LOG("__cxa_exception_cleanup\n");
    if (code != _URC_FOREIGN_EXCEPTION_CAUGHT) {
        std::terminate();
    }

    __cxa_exception* header = ((__cxa_exception*)exc - 1);
    __cxa_free_exception(header);
}

__cxa_exception* __cxa_init_exception(void* exceptionObject, std::type_info* tinfo, void (*dest)(void*))
{
    static const uint64_t EXCEPTION_CLASS = 0x50686E78432B2B00; // "PhnxC++\0"
    __cxa_exception* cxaException = cxa_exception_from_thrown_exception(exceptionObject);
    cxaException->exceptionType = tinfo;
    cxaException->exceptionDestructor = dest;
    cxaException->unwindHeader.exception_class = EXCEPTION_CLASS;
    cxaException->terminateHandler = &std::terminate;
    cxaException->unwindHeader.exception_cleanup = &__cxa_exception_cleanup;

    return cxaException;
}

void __cxa_throw(void* exceptionObject, std::type_info* tinfo, void (*dest)(void*))
{
    __cxa_exception* header = __cxa_init_exception(exceptionObject, tinfo, dest);

    __cxa_eh_globals* globals = __cxa_get_globals();
    globals->uncaughtExceptions++;

    _Unwind_RaiseException(&header->unwindHeader);

    DEBUG_LOG("_Unwind_RaiseException failed\n");
    std::terminate();
}

void* __cxa_get_exception_ptr(_Unwind_Exception* exceptionObject)
{
    return thrown_exception_from_cxa_exception(cxa_exception_from_unwind_exception(exceptionObject));
}

void* __cxa_begin_catch(_Unwind_Exception* exceptionObject)
{
    __cxa_exception* cxaException = cxa_exception_from_unwind_exception(exceptionObject);
    cxaException->handlerCount++;
    __cxa_eh_globals* globals = __cxa_get_globals();
    if (globals->caughtExceptions != cxaException) {
        cxaException->nextException = globals->caughtExceptions;
        globals->caughtExceptions = cxaException;
        globals->uncaughtExceptions--;
    }
    return thrown_exception_from_cxa_exception(cxaException);
}

void __cxa_end_catch()
{
    __cxa_eh_globals* globals = __cxa_get_globals();
    __cxa_exception* currentException = globals->caughtExceptions;

    bool rethrown = false;
    if (currentException->handlerCount < 0) {
        rethrown = true;
        currentException->handlerCount = -currentException->handlerCount;
    }

    currentException->handlerCount--;
    if (currentException->handlerCount == 0) {
        globals->caughtExceptions = currentException->nextException;
        if (!rethrown) {
            __cxa_free_exception(currentException);
        }
    }
}

std::type_info* __cxa_current_exception_type()
{
    __cxa_eh_globals* globals = __cxa_get_globals();
    __cxa_exception* currentException = globals->caughtExceptions;
    if (currentException == nullptr) {
        return nullptr;
    } else {
        return currentException->exceptionType;
    }
}

void* __cxa_rethrow()
{
    __cxa_eh_globals* globals = __cxa_get_globals();
    __cxa_exception* currentException = globals->caughtExceptions;

    if (currentException == nullptr) {
        DEBUG_LOG("No exception to rethrow");
        std::terminate();
    } else {
        // Flag exception as being rethrown
        currentException->handlerCount = -currentException->handlerCount;
    }

    return unwind_exception_from_cxa_exception(currentException);
}

std::type_info* getTypeInfo(uint64_t TTBase, int64_t offset)
{
    uint32_t* TTBasePtr = reinterpret_cast<uint32_t*>(TTBase);
    uint64_t typeInfoOffset = TTBasePtr[-offset];
    if (typeInfoOffset == 0) {
        return nullptr;
    }
    uint64_t typeInfoRef = typeInfoOffset + reinterpret_cast<uint64_t>(TTBasePtr - offset);
    return *reinterpret_cast<std::type_info**>(typeInfoRef);
}

#define DW_EE_omit           0xff
#define DW_EE_uleb128        0x01
#define DW_EE_uleb128_offset 0x9b

uint64_t unwind_dwarf_read_encoded_pointer_except_table(uint64_t* addr, uint8_t encoding, uint64_t base)
{
    switch (encoding) {
    case DW_EE_omit:
        return 0;
    case DW_EE_uleb128:
        return base + unwind_dwarf_read_uleb128(addr);
    case DW_EE_uleb128_offset:
    {
        uint64_t offset = unwind_dwarf_read_uleb128(addr);
        return *addr + offset;
    }
    default:
        DEBUG_LOG("Format type %i not supported", encoding);
        std::terminate();
    }

    return 0;
}

_Unwind_Reason_Code __gxx_personality_v0(int version, _Unwind_Action actions, uint64_t exceptionClass,
                                         _Unwind_Exception* unwind_exception, _Unwind_Context* context)
{
    (void)version;
    (void)exceptionClass;
    __cxa_exception* cxaException = cxa_exception_from_unwind_exception(unwind_exception);
    uint64_t ip = _Unwind_GetIP(context);

    uint64_t lsda = _Unwind_GetLanguageSpecificData(context);
    uint64_t LPStartEncoding = unwind_dwarf_read_u8(&lsda);
    uint64_t LPStart = unwind_dwarf_read_encoded_pointer_except_table(&lsda, LPStartEncoding, lsda);
    if (LPStart == NULL) {
        LPStart = _Unwind_GetRegionStart(context);
    }
    uint64_t TTEncoding = unwind_dwarf_read_u8(&lsda);
    uint64_t TTBase = unwind_dwarf_read_encoded_pointer_except_table(&lsda, TTEncoding, lsda);

    uint64_t callSiteOffsetEncoding = unwind_dwarf_read_u8(&lsda);
    uint64_t callSiteTableLength = unwind_dwarf_read_uleb128(&lsda);

    uint64_t callSiteTableEnd = lsda + callSiteTableLength;

    bool handlerFound = false;
    uint64_t regionLandingPad = 0;
    int actionFilterIndex = 0;
    while (lsda < callSiteTableEnd && !handlerFound) {
        // Region start/legnth
        uint64_t regionStart = unwind_dwarf_read_encoded_pointer_except_table(&lsda, callSiteOffsetEncoding, LPStart);
        uint64_t regionLength = unwind_dwarf_read_uleb128(&lsda);

        // Landing pad
        regionLandingPad = 0;
        regionLandingPad = unwind_dwarf_read_encoded_pointer_except_table(&lsda, callSiteOffsetEncoding, LPStart);
        if (regionLandingPad == LPStart) {
            regionLandingPad = 0;
        }

        // Actions
        uint64_t regionAction = unwind_dwarf_read_uleb128(&lsda);

        actionFilterIndex = 0;
        if (regionStart < ip && ip <= regionStart + regionLength) {
            uint64_t action = callSiteTableEnd + regionAction - 1;

            uint64_t actionNextEntry = 0;
            do {
                int64_t actionFilter = unwind_dwarf_read_sleb128(&action);
                if (actionFilter == 0) {
                    // Cleanup to execute
                    actionFilterIndex = 0;
                    handlerFound = true;
                    break;
                }
                actionFilterIndex++;
                std::type_info* typeInfoPtr = getTypeInfo(TTBase, actionFilter);
                if (typeInfoPtr == cxaException->exceptionType || typeInfoPtr == nullptr) {
                    // Match exception type or catch-all
                    handlerFound = true;
                    break;
                }

                actionNextEntry = unwind_dwarf_read_sleb128(&action);
                action = action + actionNextEntry - 1;
            } while (actionNextEntry != 0);
        }

        if (regionStart > ip) {
            // No handler found for this exception
            DEBUG_LOG("No handler found for this exception");
            std::terminate();
        }
    }

    if (handlerFound) {
        if (actions & _UA_SEARCH_PHASE) {
            cxaException->handlerSwitchValue = actionFilterIndex;
            // cxaException->actionRecord = ??;
            cxaException->languageSpecificData = (const char*)lsda;
            cxaException->catchTemp = (void*)regionLandingPad;
            // cxaException->adjustedPtr = ??;
            return _URC_HANDLER_FOUND;
        } else {
            if (regionLandingPad != NULL) {
                _Unwind_SetIP(context, regionLandingPad);
                _Unwind_SetGR(context, DW_X86_64_RAX, (uint64_t)unwind_exception_from_cxa_exception(cxaException));
                _Unwind_SetGR(context, DW_X86_64_RDX, actionFilterIndex);
                return _URC_INSTALL_CONTEXT;
            } else {
                return _URC_CONTINUE_UNWIND;
            }
        }
    } else {
        return _URC_NO_REASON;
    }
}

/*void __cxa_bad_cast ()
{

}

void __cxa_bad_typeid ()
{

}*/
}

namespace std {
namespace details {

void* __exc_get_cuurent_ptr()
{
    return thrown_exception_from_cxa_exception(__cxa_get_globals()->caughtExceptions);
}

void __exc_increment_refCount(void* ptr)
{
    __cxa_exception* exception = cxa_exception_from_thrown_exception(ptr);
    exception->handlerCount++;
}

void __exc_decrement_refcount(void* ptr)
{
    __cxa_exception* exception = cxa_exception_from_thrown_exception(ptr);
    exception->handlerCount--;
}

void __exc_rethrow_exception(void* ptr)
{
    abort();
}

}
}
