/*
 * current_sensor.h
 *
 *  Created on: Apr 26, 2018
 *      Author: Aakash
 *      Author: Miles
 */

#ifndef DRIVER_CURRENT_SENSOR_H_
#define DRIVER_CURRENT_SENSOR_H_

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"

#define ADC_FACTOR (5.0/4096.0)
#define CURRENT_MEASURE_RESISTANCE 1.0

void adc_init(void);

uint32_t get_adc_count(void);

float get_current(void);



#endif /* DRIVER_CURRENT_SENSOR_H_ */
