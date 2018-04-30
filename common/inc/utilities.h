#pragma once

#include <stdint.h>
#include <stddef.h> // size_t

static inline uint8_t get_checksum(uint8_t* buf, size_t len, uint8_t seed)
{
    size_t i;

    for (i = 0; i < len; i++)
    {
        seed += buf[i];
    }
    return seed;
}
