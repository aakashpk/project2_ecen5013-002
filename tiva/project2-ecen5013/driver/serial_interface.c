/*
 * serial_interface.c
 *
 *  Created on: Apr 23, 2018
 *      Author: aakash
 */

# include "driver/serial_interface.h"

void UART0_Init(uint32_t ui32SysClkFreq,uint32_t baudrate)
{
    //
    // Enable the GPIO Peripheral used by the UART.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Enable UART0
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    //
    // Configure GPIO Pins for UART mode.
    //
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    UARTConfigSetExpClk(UART0_BASE, ui32SysClkFreq, baudrate,
                (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    //
    // Initialize the UART for console I/O.
    //
    UARTStdioConfig(0, 115200, ui32SysClkFreq);
}

void UART3_Init(uint32_t ui32SysClkFreq,uint32_t baudrate)
{

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3);

    GPIOPinConfigure(GPIO_PA4_U3RX);
    GPIOPinConfigure(GPIO_PA5_U3TX);

    GPIOPinTypeUART(GPIO_PORTA_BASE,GPIO_PIN_4|GPIO_PIN_5);

    UARTConfigSetExpClk(UART3_BASE, ui32SysClkFreq, baudrate,
                (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

}


size_t uart_send_n(void * src_ptr, size_t len)
{
    unsigned char * temp;
    int i;

    temp= (unsigned char *)src_ptr;

    for(i=0;i<len;i++)
    {
        UARTCharPut(DATA_UART, * (temp+i));
    }

    return len;
}
