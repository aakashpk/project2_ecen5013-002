#pragma once

#include "packet_data_type.h"
#include "dirfile_writer.h"

typedef struct
{
    char* input_path;
    char* output_dirfile_path;
} bbb_packet_parsing_task_params_t;

void *bbb_packet_parsing_task(void* ptr);

typedef struct
{
    FILE* fp;
    int fd;
} file_wrapper_params_t;

size_t fread_wrapper_callback(void *buffer, size_t len, void *additional_params);

void fwrite_wrapper_callback(void *buffer, size_t len, void *additional_params);

void fflush_wrapper_callback(void *additional_params);


typedef struct
{
    dir_handles_t *dirfile_handles;
    // dirfile
} bbb_packet_handler_params_t;

void bbb_packet_handler_callback(packet_data_t *packet, void* additional_params);
