#pragma once

#include "packet_data_type.h"
#include "dirfile_writer.h"

typedef struct
{
    char* input_path;
    char* output_dirfile_path;
} bbb_packet_parsing_task_params_t;

void *tiva_packet_parsing_task(void* ptr);

size_t tiva_uart_read_wrapper_callback(void *buffer, size_t len, void *additional_params);

void tiva_uart_write_wrapper_callback(void *buffer, size_t len, void *additional_params);

void tiva_packet_handler_callback(packet_data_t *packet, void* additional_params);
