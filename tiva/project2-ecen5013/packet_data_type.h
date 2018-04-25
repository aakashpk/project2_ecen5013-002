/*
 * packet_data_type.h
 *
 *  Created on: Apr 23, 2018
 *      Author: Aakash
 *       Author: Miles
 */

/**  NOTES FROM 22 April
 * 32-bit float for everything

struct PID parameters (somewhat static, unless updated or auto tuned)
// standalone version of this sent from GUI to TIVA
Kp
Kd
Ki

struct PID status
// TIVA -> GUI
Setpoint
// Desired RPS
Measured
// Observed RPS
Error
// Measured - Setpoint
Output
// PWM on-time (assuming period stays the same)
// 8-bit int should be fine, but 32-bit more friendly for architecture and alignment
// if we want float version, we can just add components in gui
Components
// these are constantly changing based on current and previous errors
// The sum of all of these is equal to output
P
Accumulated I
D
struct PID parameters
// shouldn’t change in most cases
// might be somewhat redundant for non-auto-tuning
// but still good if connecting to pre-configured system
// Could just limit struct

struct heartbeat
    // no payload - just a type

struct other_configuration
auto-tuning enabled
update rate
wind-up integration limit

struct packet
type
length
time?
// Not required for kst time series data, but could be nice if
union payload
struct pid_parameters update // GUI -> TIVA
struct other configuration
struct pid status // TIVA -> GUI
// struct heartbeat // either direction, no payload
 *
 */

#ifndef PACKET_DATA_TYPE_H_
#define PACKET_DATA_TYPE_H_

#include <stdint.h>
#include <stdbool.h>
#include "FreeRTOS.h"

typedef enum packet_type
{
    UNINITIALISED,
    HEARTBEAT, // Only packet type and timestamp
    MOTOR_VALUES,  // motor speed, setpoint, error, pid values
    PARAMETERS, // pid parameters
    CONFIGUARTION, // pid config data

}packet_type_t;

extern char *packet_type_strings[];

typedef struct packet_header_type
{
    size_t length;// TODO: Is a length required here ??
    packet_type_t packet_type;
    TickType_t timestamp;
    char foo;

}packet_header_t;


typedef struct motor_values
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

}motor_values_t;

typedef struct pid_parameters
{
    float kp;
    float ki;
    float kd;
}pid_param_t;

typedef struct
{
    bool auto_tune; // auto tune enabled
    uint32_t rate_s; //update rate in seconds
    float windup_limit; // Integral windup limit
}pid_config_t;

typedef struct data_packet
{
    packet_header_t header;


    // padding risk

    union
    {
        motor_values_t value;
        pid_param_t parameters;
        pid_config_t configuration;
    } ;


}packet_data_t;

#endif /* PACKET_DATA_TYPE_H_ */





























