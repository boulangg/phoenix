#ifndef __STDATOMIC_H
#define __STDATOMIC_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {

#include <stdbool.h>

#endif

#define _Atomic(X) X

/* 7.17.1 Introduction */

#define ATOMIC_BOOL_LOCK_FREE     /* implementation-defined */
#define ATOMIC_CHAR_LOCK_FREE     /* implementation-defined */
#define ATOMIC_CHAR16_T_LOCK_FREE /* implementation-defined */
#define ATOMIC_CHAR32_T_LOCK_FREE /* implementation-defined */
#define ATOMIC_WCHAR_T_LOCK_FREE  /* implementation-defined */
#define ATOMIC_SHORT_LOCK_FREE    /* implementation-defined */
#define ATOMIC_INT_LOCK_FREE      /* implementation-defined */
#define ATOMIC_LONG_LOCK_FREE     /* implementation-defined */
#define ATOMIC_LLONG_LOCK_FREE    /* implementation-defined */
#define ATOMIC_POINTER_LOCK_FREE  /* implementation-defined */

/* 7.17.2 Initialization */

#define ATOMIC_VAR_INIT(value) (value)
#define atomic_init (void)

/* 7.17.3 Order and consistency */

	enum memory_order
	{
		memory_order_relaxed = __ATOMIC_RELAXED,
		memory_order_consume = __ATOMIC_CONSUME,
		memory_order_acquire = __ATOMIC_ACQUIRE,
		memory_order_release = __ATOMIC_RELEASE,
		memory_order_acq_rel = __ATOMIC_ACQ_REL,
		memory_order_seq_cst = __ATOMIC_SEQ_CST
	};

#define kill_dependency(y) (y)

/* 7.17.4 Fences */

#define atomic_thread_fence(order) __atomic_thread_fence(order)
#define atomic_signal_fence(order) __atomic_signal_fence(order)

/* 7.17.5 Lock-free property */

#define atomic_is_lock_free(obj) __atomic_is_lock_free(obj)

/* 7.17.6 Atomic integer types */

	typedef _Atomic(_Bool)atomic_bool;
	typedef _Atomic(char)atomic_char;
	typedef _Atomic(signed char)atomic_schar;
	typedef _Atomic(unsigned char)atomic_uchar;
	typedef _Atomic(short)atomic_short;
	typedef _Atomic(unsigned short)atomic_ushort;
	typedef _Atomic(int)atomic_int;
	typedef _Atomic(unsigned int)atomic_uint;
	typedef _Atomic(long)atomic_long;
	typedef _Atomic(unsigned long)atomic_ulong;
	typedef _Atomic(long long)atomic_llong;
	typedef _Atomic(unsigned long long) atomic_ullong;
	typedef _Atomic(uint_least16_t)atomic_char16_t;
	typedef _Atomic(uint_least32_t)atomic_char32_t;
	typedef _Atomic(wchar_t)atomic_wchar_t;
	typedef _Atomic(int_least8_t)atomic_int_least8_t;
	typedef _Atomic(uint_least8_t)atomic_uint_least8_t;
	typedef _Atomic(int_least16_t)atomic_int_least16_t;
	typedef _Atomic(uint_least16_t)atomic_uint_least16_t;
	typedef _Atomic(int_least32_t)atomic_int_least32_t;
	typedef _Atomic(uint_least32_t)atomic_uint_least32_t;
	typedef _Atomic(int_least64_t)atomic_int_least64_t;
	typedef _Atomic(uint_least64_t)atomic_uint_least64_t;
	typedef _Atomic(int_fast8_t)atomic_int_fast8_t;
	typedef _Atomic(uint_fast8_t)atomic_uint_fast8_t;
	typedef _Atomic(int_fast16_t)atomic_int_fast16_t;
	typedef _Atomic(uint_fast16_t)atomic_uint_fast16_t;
	typedef _Atomic(int_fast32_t)atomic_int_fast32_t;
	typedef _Atomic(uint_fast32_t)atomic_uint_fast32_t;
	typedef _Atomic(int_fast64_t)atomic_int_fast64_t;
	typedef _Atomic(uint_fast64_t)atomic_uint_fast64_t;
	typedef _Atomic(intptr_t)atomic_intptr_t;
	typedef _Atomic(uintptr_t)atomic_uintptr_t;
	typedef _Atomic(size_t)atomic_size_t;
	typedef _Atomic(ptrdiff_t)atomic_ptrdiff_t;
	typedef _Atomic(intmax_t)atomic_intmax_t;
	typedef _Atomic(uintmax_t)atomic_uintmax_t;

	/* 7.17.7 Operations on atomic types */

#define atomic_store(object, desired) __atomic_store(object, desired, memory_order_seq_cst)
#define atomic_store_explicit(object, desired, order) __ATOMIC_store(object, desired, order)

#define atomic_load(object) __atomic_load(object, memory_order_seq_cst)
#define atomic_load_explicit(object, order) __atomic_load(object, order)

#define atomic_exchange(object, desired) __atomic_exchange(object, desired, memory_order_seq_cst)
#define atomic_exchange_explicit(object, desired, order) __atomic_exchange(object, desired, order)

#define atomic_compare_exchange_strong(object, expected, desired) __atomic_compare_exchange(object, expected, desired, false, memory_order_seq_cst, memory_order_seq_cst)
#define atomic_compare_exchange_strong_explicit(object, expected, desired, success, failure) __atomic_compare_exchange(object, expected, desired, false, success, failure)
#define atomic_compare_exchange_weak(object, expected, desired) __atomic_compare_exchange(object, expected, desired, true, memory_order_seq_cst, memory_order_seq_cst)
#define atomic_compare_exchange_weak_explicit(object, expected, desired, success, failure) __atomic_compare_exchange(object, expected, desired, true, success, failure)

#define atomic_fetch_add(object, operand) __atomic_fetch_add(object, operand, memory_order_seq_cst)
#define atomic_fetch_add_explicit(object, operand, order) __atomic_fetch_add(object, operand, order)

#define atomic_fetch_sub(object, operand) __atomic_fetch_sub(object, operand, memory_order_seq_cst)
#define atomic_fetch_sub_explicit(object, operand, order) __atomic_fetch_sub(object, operand, order)

#define atomic_fetch_or(object, operand) __atomic_fetch_or(object, operand, memory_order_seq_cst)
#define atomic_fetch_or_explicit(object, operand, order) __atomic_fetch_or(object, operand, order)

#define atomic_fetch_xor(object,  operand) __atomic_fetch_xor(object, operand, memory_order_seq_cst)
#define atomic_fetch_xor_explicit(object, operand, order) __atomic_fetch_xor(object, operand, order)

#define atomic_fetch_and(object, operand) __atomic_fetch_and(object, operand, memory_order_seq_cst)
#define atomic_fetch_and_explicit(object, operand, order) __atomic_fetch_and(object, operand, order)

/* 7.17.8 Atomic flag type and operations */

	struct atomic_flag
	{
		bool state;
	};

#define ATOMIC_FLAG_INIT { false }

#define atomic_flag_test_and_set(obj) __atomic_flag_test_and_set(obj, memory_order_seq_cst)
#define atomic_flag_test_and_set_explicit(obj, order) __atomic_flag_test_and_set(obj, order)

#define atomic_flag_clear(object) __atomic_flag_clear(object, memory_order_seq_cst)
#define atomic_flag_clear_explicit(object, order) __atomic_flag_clear(object, order)


#ifdef __cplusplus
}
#endif

#endif
