#include <stdint.h>
#include <stddef.h> // size_t

uint8_t get_checksum(uint8_t* buf, size_t len, uint8_t seed)
{
    for (size_t i = 0; i < len; i++)
    {
        seed += buf[i];
    }
    return seed;
}
