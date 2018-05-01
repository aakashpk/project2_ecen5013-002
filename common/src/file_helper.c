#include "file_helper.h"

#include <sys/stat.h>
#include <string.h>
#include <stdlib.h> // abort


FILE *fopen_check(const char *path, char *flags)
{
    FILE *fp = fopen(path, flags);
    if (!fp)
    {
        perror("fopen_check failed to open file");
        abort();
    }
    return fp;
}

#define PATH_LENGTH_LIMIT 256
FILE *fopen_dir_check(const char *dir, const char *name, char *flags)
{
    char path[PATH_LENGTH_LIMIT];
    strncpy(path, dir, PATH_LENGTH_LIMIT);
    strncat(path, "/", PATH_LENGTH_LIMIT);
    strncat(path, name, PATH_LENGTH_LIMIT);

    struct stat st = {0};
    if (stat(dir, &st) == -1)
    {
        mkdir(dir, 0700);
    }

    return fopen_check(path, flags);
}