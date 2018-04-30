/*
 * serial_interface.h
 *
 *  Created on: Apr 23, 2018
 *      Author: aakash
 * Author: Miles
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
 * @brief Send characters from a 
 * 
 * @param src_ptr 
 * @param len 
 * @return size_t 
 */
size_t uart_send_n(void * src_ptr, size_t len);

/**
 * @brief read len no of characters from UART
 * and write to a buffer
 * 
 * @param src_ptr 
 * @param len number of characters to be read
 * @return size_t number of characters written to buffer
 */
size_t uart_get_n(void * src_ptr, size_t len);

/**
 * @brief Write data from UART into buffer 
 * 
 * @param buffer buffer
 * @param max_length maximum length of buffer
 * @return size_t number of bytes written
 */
size_t uart_fill_buffer(void *buffer, size_t max_length);

/**
 * @brief 
 * 
 */
void UART3IntHandler(void);

#endif /* DRIVER_SERIAL_INTERFACE_H_ */
