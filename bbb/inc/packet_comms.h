#pragma once

#include <stdio.h>
#include <time.h>

#include "packet_data_type.h"

/**
 * @brief 
 * 
 * @param packet 
 */
void print_data_packet(packet_data_t * packet);

size_t get_heartbeat(packet_data_t * packet);

size_t get_motor_values(packet_data_t * packet);

size_t get_pid_params(packet_data_t * packet);

size_t get_pid_config(packet_data_t * packet);

size_t calc_checksum(packet_type_t type, packet_data_t * packet);