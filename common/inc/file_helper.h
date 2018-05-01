/**
 * @brief Convenience function for file IO
 *
 * @file file_helper.h
 * @author Aakash Kumar @author Miles Frain
 * @date 2018-04-29
 */

#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

/**
 * @brief Write and (optionally) flush file
 *
 * @param fp file pointer
 * @param data to write
 * @param len bytes to write
 * @param force_flush whether to force a flush
 */
static inline void fwrite_flush(FILE *fp, const void *data, size_t len, bool force_flush)
{
    assert(1 == fwrite(data, len, 1, fp));

    if (force_flush)
    {
        assert(0 == fflush(fp));
    }
}


/**
 * @brief open file and assert failure
 *
 * @param path to file
 * @param flags when opening file
 * @return FILE* file pointer
 */
FILE *fopen_check(const char *path, char *flags);

/**
 * @brief open file in directory and assert failure
 *
 * @param dir directory containing file
 * @param path to file
 * @param flags when opening file
 * @return FILE* file pointer
 */
FILE *fopen_dir_check(const char *dir, const char *name, char *flags);