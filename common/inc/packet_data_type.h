/*
 * packet_data_type.h
 *
 *  Created on: Apr 23, 2018
 *      Author: Aakash
 *      Author: Miles
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef enum {
    UNINITIALISED,
    COMM_HEARTBEAT, // Only packet type and timestamp
    MOTOR_VALUES,   // motor speed, setpoint, error, pid values
    PID_PARAMETERS,     // pid parameters
    PID_CONFIGUARTION,  // pid config data
    NUM_PACKET_TYPES,

} packet_type_t;

typedef struct
{
    uint32_t length; // Not required, since we can derrive length from packet type.
    uint32_t packet_type;
    uint32_t timestamp;

} packet_header_t;

typedef struct
{
    float speed; // motor speed in rps, read from tachometer
    float setpoint; //required speed in rps
    float error;

    // Output to be written to pwm for motor control
    uint32_t pwm_output;

    // Calculated p, i and d terms
    float p_value;
    float i_value;
    float d_value;

    float current;

}motor_values_t;

typedef struct
{
    float kp;
    float ki;
    float kd;

} pid_param_t;

typedef struct
{
    uint32_t auto_tune; // auto tune enabled
    uint32_t update_period_ns; //update period
    float windup_limit; // Integral windup limit

} pid_config_t;

typedef struct
{
    packet_header_t header;

    // padding risk

    union
    {
        motor_values_t motor_values;
        pid_param_t pid_param;
        pid_config_t pid_config;
        uint32_t payload; // convenience handle to union address
        // Above "payload" is less confusing than pointing to other types to get
        // this address for the generic case, and avoids using named union,
        // which makes children more annoying to access.
    } ;

} packet_data_t;

// extern (implicit with const)
extern const uint32_t magic_num;

// remove stddef above if this needs to move
extern const size_t packet_payload_size[NUM_PACKET_TYPES];

extern const char *packet_type_strings[NUM_PACKET_TYPES];