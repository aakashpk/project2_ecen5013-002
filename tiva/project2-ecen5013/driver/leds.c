/*
 * leds.c
 *
 *  Created on: Apr 23, 2018
 *      Author: aakash
 * Author: Miles
 */

#include "driver/leds.h"

void ConfigureLEDs(void)
{
    // Enable GPIO for LED pins and set pins to output
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, LED1|LED2);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, LED3);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, LED4);

    GPIOPinWrite(GPIO_PORTN_BASE, LED1|LED2, 0x00);
    GPIOPinWrite(GPIO_PORTF_BASE, LED3, 0x00);
    GPIOPinWrite(GPIO_PORTF_BASE, LED4, 0x00);
}
