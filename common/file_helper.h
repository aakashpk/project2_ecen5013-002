#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

static inline void fwrite_flush(FILE *fp, void *data, size_t len, bool force_flush)
{
    assert(1 == fwrite(data, len, 1, fp));

    if (force_flush)
    {
        assert(0 == fflush(fp));
    }
}

FILE *fopen_check(const char *path, char *flags);

FILE *fopen_dir_check(const char *dir, const char *name, char *flags);