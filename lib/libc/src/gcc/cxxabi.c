#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void sys_exit(int) __attribute__((weak));

__extension__ typedef int __guard __attribute__((mode(__DI__)));

#define ATEXIT_MAX_FUNCS	128

typedef struct atexit_func_entry_t {
	void (*destructor_func)(void *);
	void *obj_ptr;
	void *dso_handle;
} atexit_func_entry_t;

int __cxa_guard_acquire (__guard *);
void __cxa_guard_release (__guard *);
void __cxa_guard_abort (__guard *);
int __cxa_atexit(void (*f)(void *), void *objptr, void *dso);
void __cxa_finalize(void *f);
int atexit(void (*f)(void));
void __cxa_pure_virtual();

void __cxa_pure_virtual()
{
    // Do nothing or print an error message.
}

//void *__dso_handle = 0;

atexit_func_entry_t __atexit_funcs[ATEXIT_MAX_FUNCS];
int64_t __atexit_func_count = 0;

int __cxa_guard_acquire (__guard *g) {
	return !*(char *)(g);
}

void __cxa_guard_release (__guard *g) {
	*(char *)g = 1;
}

void __cxa_guard_abort (__guard *g) {
	(void)g;
}

int __cxa_atexit(void (*f)(void *), void *objptr, void *dso) {
	if (__atexit_func_count >= ATEXIT_MAX_FUNCS) {
		return -1;
	}
	__atexit_funcs[__atexit_func_count].destructor_func = f;
	__atexit_funcs[__atexit_func_count].obj_ptr = objptr;
	__atexit_funcs[__atexit_func_count].dso_handle = dso;
	__atexit_func_count++;
	return 0;
}

void __cxa_finalize(void *f) {
	int64_t i = __atexit_func_count;
	if (!f) {
		while (i--) {
			if (__atexit_funcs[i].destructor_func) {
				(*__atexit_funcs[i].destructor_func)(__atexit_funcs[i].obj_ptr);
			}
		}
		return;
	}

	for ( ; i >= 0; --i) {
		if (__atexit_funcs[i].destructor_func == f) {
			(*__atexit_funcs[i].destructor_func)(__atexit_funcs[i].obj_ptr);
			__atexit_funcs[i].destructor_func = 0;
		}
	}
}

int atexit(void (*f)(void)) {
	typedef void (*fn_ptr)(void*);
	return __cxa_atexit((fn_ptr)f, NULL, NULL);
}

void exit(int exit_code) {
	__cxa_finalize(NULL);
	sys_exit(exit_code);
	__builtin_unreachable();
}

#ifdef __cplusplus
};
#endif
