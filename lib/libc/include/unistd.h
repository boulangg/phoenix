#ifndef __UNISTD__
#define __UNISTD__



#ifdef __cplusplus
extern "C" {
#endif

int sys_fork();

int fork() { return sys_fork(); }

#ifdef __cplusplus
}
#endif

#endif
