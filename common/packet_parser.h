#pragma once

#include "dirfile_writer.h"

typedef struct
{
    char* input_path;
    char* output_dir;
    // Todo - should configure read mode
    // hopefully output through same interface is independent - should be allowed
} packet_parser_params_t;

void *packet_parser_task(void* ptr);