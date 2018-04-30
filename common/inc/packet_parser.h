/**
 * @brief Parses incomming packet stream
 *
 * @file packet_parser.h
 * @author Aakash Kumar @author Miles Frain
 * @date 2018-04-29
 */

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