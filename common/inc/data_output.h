/**
 * @brief Abstraction for interacting with a variety of IO options.
 *
 * @file data_output.h
 * @author Aakash Kumar
 * @author Miles Frain
 * @date 2018-04-29
 */

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

/**
 * @brief Read from a variety of sources
 *
 * @param output handle
 * @param data read buffer
 * @param len maximum number of bytes to read
 * @return size_t bytes read
 */
size_t data_output_read(data_output_t *output, char* data, size_t len);

/**
 * @brief Write to a variety of destinations
 *
 * @param output handle
 * @param data contents to write
 * @param len number of bytes to write
 */
void data_output_write(data_output_t *output, char* data, size_t len);

/**
 * @brief Open a variety of IO sources
 *
 * @param output handle
 * @param name IO source identifier
 */
void data_output_open(data_output_t *output, char* name);

/**
 * @brief Close a variety of IO sources
 *
 * @param output handle
 */
void data_output_close(data_output_t *output);

/**
 * @brief Flush a variety of IO sources
 *
 * @param output handle
 */
void data_output_flush(data_output_t *output);
