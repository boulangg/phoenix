/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include <link.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/auxv.h>
#include <unistd.h>

int main(int argc, char* argv[], char* envp[])
{
    for (int i = 0; i < argc; i++) {
        printf("arg %i: %s\n", i, argv[i]);
    }
    printf("\n");

    int envpCount = 0;
    while (envp[envpCount] != NULL) {
        printf("%s\n", envp[envpCount]);
        envpCount++;
    }
    printf("\n");

    const char* username = getenv("USER");
    if (username != NULL) {
        printf("USER: %s\n", username);
    }
    printf("\n");

    printf("AT_PAGESZ: 0x%p\n", (void*)getauxval(AT_PAGESZ));
    printf("AT_ENTRY:  0x%p\n", (void*)getauxval(AT_ENTRY));
    printf("AT_PHDR:   0x%p\n", (void*)getauxval(AT_PHDR));
    printf("AT_PHENT:  %lu (bytes)\n", getauxval(AT_PHENT));
    printf("AT_PHNUM:  %lu\n", getauxval(AT_PHNUM));

    dl_iterate_phdr(
        [](struct dl_phdr_info* info, size_t size, void* data)
        {
            (void)size;
            (void)data;
            printf("%s, %p, %p, %i\n", info->dlpi_name, (void*)info->dlpi_addr, info->dlpi_phdr, info->dlpi_phnum);
            return 0;
        },
        nullptr);

    return 0;
}
