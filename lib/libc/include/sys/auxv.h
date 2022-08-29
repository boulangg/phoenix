#pragma once

#include <stdint.h>

#define AT_NULL   	0		/* end of vector */
#define AT_IGNORE 	1		/* entry should be ignored */
#define AT_EXECFD 	2		/* file descriptor of program */
#define AT_PHDR   	3		/* program headers for program */
#define AT_PHENT  	4		/* size of program header entry */
#define AT_PHNUM  	5		/* number of program headers */
#define AT_PAGESZ 	6		/* system page size */
#define AT_BASE   	7		/* base address of interpreter */
#define AT_FLAGS  	8		/* flags */
#define AT_ENTRY  	9		/* entry point of program */
#define AT_NOTELF 	10		/* program is not ELF */
#define AT_UID    	11		/* real uid */
#define AT_EUID   	12		/* effective uid */
#define AT_GID    	13		/* real gid */
#define AT_EGID   	14		/* effective gid */

typedef struct
{
	uint64_t a_type;
	union
	{
		uint64_t a_val;
		void* a_ptr;
		void (*a_fnc)();
	} a_un;
} auxv_t;

#ifdef __cplusplus
extern "C" {
#endif

	extern uint64_t getauxval(uint64_t __type);

#ifdef __cplusplus
}
#endif
