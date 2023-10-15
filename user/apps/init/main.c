/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include <unistd.h>

#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;
    char buffer[1024];
    getcwd(buffer, 1024);
    printf("cwd: %s\n", buffer);
    chdir("/bin");
    getcwd(buffer, 1024);
    printf("cwd: %s\n", buffer);
    if (fork() == 0) {
        char* const argv[3] = {(char*)"test1", (char*)"1", 0};
        char* const envp[1] = {0};
        execve("test1", argv, envp);
        return 10;
    } else {
        char* const argv[3] = {(char*)"ls", (char*)"/", 0};
        char* const envp[1] = {0};
        execve("/bin/ls", argv, envp);
        return getpid();
    }
}
