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

void quad_encoder_init(void);

uint32_t get_position(void);

#endif /* SPEED_SENSOR_H_ */
