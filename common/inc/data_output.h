#pragma once

#include <stdio.h>

typedef enum
{
    OUTPUT_TO_FILE,
    OUTPUT_TO_SERIAL,
    OUTPUT_TO_SOCKET,
    NUM_OUTPUT_MODES,
} data_output_mode_t;

typedef struct
{
    data_output_mode_t current_output_mode;

    union // output_handle
    {
        FILE *output_fp;
        // todo serial
        // *output_serial_p;
        // todo socket
        // *output_socket_p;
    };
} data_output_t;

void data_output_write(data_output_t *output, char* data, size_t len);

void data_output_open(data_output_t *output, char* name);

void data_output_close(data_output_t *output);

void data_output_flush(data_output_t *output);

