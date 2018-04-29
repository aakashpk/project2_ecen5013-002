/*
 * serial_interface.h
 *
 *  Created on: Apr 23, 2018
 *      Author: aakash
 */

#ifndef DRIVER_SERIAL_INTERFACE_H_
#define DRIVER_SERIAL_INTERFACE_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"

#include "utils/uartstdio.h"

#include "FreeRTOS.h"
#include "semphr.h"

extern SemaphoreHandle_t xUARTRxEventSemaphore;

#define DATA_UART UART3_BASE

void UART0_Init(uint32_t ui32SysClkFreq,uint32_t baudrate);
void UART3_Init(uint32_t ui32SysClkFreq,uint32_t baudrate);

/**
 *
 */
size_t uart_send_n(void * src_ptr, size_t len);


size_t uart_get_n(void * src_ptr, size_t len);

/**
 * UART Interrupt handler
 */
void UART3IntHandler(void);

#endif /* DRIVER_SERIAL_INTERFACE_H_ */
