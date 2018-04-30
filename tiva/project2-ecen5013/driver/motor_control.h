/*
 * motor_control.h
 *
 *  Created on: Apr 27, 2018
 *      Author: aakash
 * Author: Miles
 */

#ifndef MOTOR_CONTROL_H_
#define MOTOR_CONTROL_H_

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/pwm.h"
#include "packet_data_type.h"

//PID library from https://github.com/mblythe86/C-PID-Library
#include "pid.h"

#define KP_DEFAULT_VAL 1
#define KI_DEFAULT_VAL 0
#define KD_DEFAULT_VAL 0

#define OUPUT_MIN_VAL 20
#define OUPUT_MAX_VAL 100

extern motor_values_t gMotorValues; // from packet datatype
extern pid_param_t g_pid_params;
extern PidType g_pid_values; // from PID library

/**
 * @brief Initialize motor PWM
 * 
 */
void pwm_init(void);

/**
 * @brief write pwm duty cycle to motor 
 * 
 * @param duty_cycle 
 */
void motor_speed(uint32_t duty_cycle);

/**
 * @brief Initialize PID 
 * 
 */
void Init_PID(void);

/**
 * @brief Coumputes pid output values
 * 
 * @return uint32_t 
 */
uint32_t PID_compute_output(void);

/**
 * @brief test function for PWM
 * 
 * @param value 
 */
void led_bright(uint8_t value); // testing function


#endif /* MOTOR_CONTROL_H_ */
