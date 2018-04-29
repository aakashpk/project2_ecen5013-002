void fwrite_flush(FILE *fp, void *data, size_t len, bool force_flush)
{
    assert(1 == fwrite(data, len, 1, fp));

    if (force_flush)
    {
        assert(0 == fflush(fp));
    }
}

FILE *fopen_check(const char *path, char *flags)
{
    FILE *fp;
    assert((fp = fopen(path, flags)));
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