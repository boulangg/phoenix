/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef _DIRENT_H_
#define _DIRENT_H_

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct DIR DIR;
#define NAME_MAX 255

struct dirent
{
    ino_t d_ino;               /* numéro d'inœud */
    char d_name[NAME_MAX + 1]; /* nom du fichier */
                               // off_t          d_off;       /* décalage jusqu'à la dirent suivante */
    // unsigned short d_reclen;    /* longueur de cet enregistrement */
    // unsigned char  d_type;      /* type du fichier */
};

int closedir(DIR*);
DIR* opendir(const char*);
struct dirent* readdir(DIR*);
int readdir_r(DIR*, struct dirent*, struct dirent**);
void rewinddir(DIR*);
void seekdir(DIR*, long int);
long int telldir(DIR*);

#ifdef __cplusplus
}
#endif

#endif
