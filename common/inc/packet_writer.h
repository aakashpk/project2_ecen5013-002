/**
 * @brief writes encoded packet to stream
 *
 * @file packet_writer.h
 * @author Aakash Kumar @author Miles Frain
 * @date 2018-04-29
 */
#pragma once

#include "packet_data_type.h"

typedef void (*write_callback_t)(void *buffer, size_t len, void *additional_params);

typedef void (*flush_callback_t)(void *additional_params);

void write_packet(packet_data_t *data,
                  write_callback_t write_callback,
                  flush_callback_t flush_callback,
                  void* additional_write_flush_params);
