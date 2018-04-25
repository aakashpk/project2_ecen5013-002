/*
 * leds.h
 *
 *  Created on: Apr 23, 2018
 *      Author: aakash
 */

#ifndef LEDS_H_
#define LEDS_H_

#define LED1 GPIO_PIN_1
#define LED2 GPIO_PIN_0

#define LED3 GPIO_PIN_4
#define LED4 GPIO_PIN_0

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"

#define LEDOFF(x) GPIOPinWrite(GPIO_PORTN_BASE, (x), 0x01<<((uint8_t)(x)))
#define LEDON(x) GPIOPinWrite(GPIO_PORTN_BASE, (x), ~(0x01<<((uint8_t)(x))))

#define LED_OFF(x) GPIOPinWrite(GPIO_PORTF_BASE, (x), 0x01<<((uint8_t)(x)))
#define LED_ON(x) GPIOPinWrite(GPIO_PORTF_BASE, (x), ~(0x01<<((uint8_t)(x))))

void ConfigureLEDs(void);

#endif /* LEDS_H_ */
