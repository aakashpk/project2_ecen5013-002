/*
 * speed_sensor.h
 *
 *  Created on: Apr 26, 2018
 *      Author: aakash
 */

#ifndef SPEED_SENSOR_H_
#define SPEED_SENSOR_H_

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/qei.h"
#include "driverlib/timer.h"


#define PULSES_PER_REV  16
#define PERIODS_PER_SEC 250

void quad_encoder_init(void);

uint32_t get_position(void);

void tachometer_init(void);

void GPIOPP3IntHandler(void);


float get_speed(void);


float get_speed_setpoint(void);


float get_error(void);

uint32_t get_speed_test(void);

#endif /* SPEED_SENSOR_H_ */
