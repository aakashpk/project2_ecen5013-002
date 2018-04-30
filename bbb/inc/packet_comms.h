/**
 * @brief 
 * 
 * @file packet_comms.h
 * @author Aakash
 * @author Miles
 * @date 2018-04-29
 */
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

/**
 * @brief Get the heartbeat object
 * 
 * @param packet 
 * @return size_t 
 */
size_t get_heartbeat(packet_data_t * packet);

/**
 * @brief Get the motor values object
 * 
 * @param packet 
 * @return size_t 
 */
size_t get_motor_values(packet_data_t * packet);

/**
 * @brief Get the pid params object
 * 
 * @param packet 
 * @return size_t 
 */
size_t get_pid_params(packet_data_t * packet);

/**
 * @brief Get the pid config object
 * 
 * @param packet 
 * @return size_t 
 */
size_t get_pid_config(packet_data_t * packet);

/**
 * @brief 
 * 
 * @param type 
 * @param packet 
 * @return size_t 
 */
size_t calc_checksum(packet_type_t type, packet_data_t * packet);