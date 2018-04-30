/**
 * @brief writes encoded packet to stream
 *
 * @file packet_writer.h
 * @author Aakash Kumar @author Miles Frain
 * @date 2018-04-29
 */
#pragma once

#include "packet_data_type.h"
#include "data_output.h"

void write_packet(data_output_t *output, packet_data_t *data);
