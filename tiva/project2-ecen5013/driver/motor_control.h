/*
 * motor_control.h
 *
 *  Created on: Apr 27, 2018
 *      Author: aakash
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

void pwm_init(void);

void motor_speed(uint8_t value);

void led_bright(uint8_t value); // testing function

float get_pvalue(void);


float get_ivalue(void);


float get_dvalue(void);

float get_kp(void);


float get_ki(void);


float get_kd(void);



#endif /* MOTOR_CONTROL_H_ */
