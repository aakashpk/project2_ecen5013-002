/*
 * speed_sensor.h
 *
 *  Created on: Apr 26, 2018
 *      Author: aakash
 * Author: Miles
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

/**
 * @brief Initialize the quad decoder
 * 
 */
void quad_encoder_init(void);

/**
 * @brief Get the position object
 * 
 * @return uint32_t 
 */
uint32_t get_position(void);

/**
 * @brief Initialize GPIO interrupt for 
 * single pulse tachometer reads
 * 
 */
void tachometer_init(void);

/**
 * @brief GPIO interrupt handler for 
 * tachometer pulses
 */
void GPIOPP3IntHandler(void);

/**
 * @brief Get the speed read from quad decode calculation
 * 
 * @return float speed in rps
 */
float get_speed(void);

/**
 * @brief test function for debug
 * 
 * @return uint32_t 
 */
uint32_t get_speed_test(void);

#endif /* SPEED_SENSOR_H_ */
