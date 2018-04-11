/*
 * driverInit.h
 *
 *  Created on: Apr 2, 2018
 *      Author: aakash
 */

#ifndef DRIVERINIT_H_
#define DRIVERINIT_H_

#include <stdint.h>
#include <stdbool.h>

#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"

#define LED1 GPIO_PIN_1
#define LED2 GPIO_PIN_0

void ConfigureUART(uint32_t g_ui32SysClock);


void ConfigureLED(void);


#endif /* DRIVERINIT_H_ */
