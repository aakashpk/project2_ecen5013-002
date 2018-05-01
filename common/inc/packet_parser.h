/**
 * @brief Parses incomming packet stream
 *
 * @file packet_parser.h
 * @author Aakash Kumar @author Miles Frain
 * @date 2018-04-29
 */

#pragma once

#include "packet_data_type.h"
//#include <stddef.h> // size_t

typedef size_t (*read_callback_t)(void *buffer, size_t len, void *additional_params);

typedef void (*packet_handler_callback_t)(packet_data_t *packet, void* additional_params);

void packet_parser(uint8_t *buffer,
                   size_t buffer_size,
                   read_callback_t read_callback,
                   void *read_params,
                   packet_handler_callback_t packet_handler_callback,
                   void *packet_handler_params);
