/*
 * packet_comm.h
 *
 *  Created on: Apr 23, 2018
 *      Author: Aakash
 *      Author: Miles
 */

#ifndef PACKET_COMM_H_
#define PACKET_COMM_H_

#include <stdio.h>

#include "packet_data_type.h"
#include "utils/uartstdio.h"
#include "packet_data_type.h"
#include "driver/serial_interface.h"

#include "driver/speed_sensor.h"
#include "driver/motor_control.h"

#include "FreeRTOS.h"
#include "FreeRTOS_Sockets.h"




typedef enum comms_hardware
{
    UNINIT,
    UART,
    SPI,
    TCP,
}comm_hw_t;

extern volatile comm_hw_t comm_hw_used;

/**
 *
 */
int packet_send(packet_type_t type, packet_data_t * packet);

/**
 *
 */
int packet_recieve( packet_data_t * packet);


size_t get_heartbeat(packet_data_t * packet);


size_t get_motor_values(packet_data_t * packet);


size_t get_pid_params(packet_data_t * packet);


size_t get_pid_config(packet_data_t * packet);


size_t calc_checksum(packet_type_t type, packet_data_t * packet);


void print_data_packet(packet_data_t * packet);


#endif /* PACKET_COMM_H_ */
