#ifndef __UNISTD__
#define __UNISTD__



#ifdef __cplusplus
extern "C" {
#endif

int sys_fork();
int sys_execve(const char *file, char *const argv[], char *const envp[]);
int sys_getpid();

int fork() { return sys_fork(); }
int execve(const char *file, char *const argv[], char *const envp[]) { return sys_execve(file, argv, envp); }
int getpid() { return sys_getpid(); }

#ifdef __cplusplus
}
#endif

#endif
