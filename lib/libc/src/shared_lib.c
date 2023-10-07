#ifdef IS_SHARED_LIB

typedef void (*func_ptr)();
func_ptr __preinit_array_start[0], __preinit_array_end[0];
func_ptr __init_array_start[0], __init_array_end[0];
func_ptr __fini_array_start[0], __fini_array_end[0];

#endif
