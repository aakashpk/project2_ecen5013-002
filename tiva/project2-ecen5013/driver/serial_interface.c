/*
 * serial_interface.c
 *
 *  Created on: Apr 23, 2018
 *      Author: aakash
 */

#include "driver/serial_interface.h"

#include "driver/leds.h"

//TODO: this should be removed
#include "packet_data_type.h"
#include "packet_comm.h"

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

    // Wait for the UART0 module to be ready.
    //
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0))
    { }
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

    // Wait for the UART0 module to be ready.
    //
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART3))
    { }


    GPIOPinConfigure(GPIO_PA4_U3RX);
    GPIOPinConfigure(GPIO_PA5_U3TX);

    GPIOPinTypeUART(GPIO_PORTA_BASE,GPIO_PIN_4|GPIO_PIN_5);

    UARTConfigSetExpClk(UART3_BASE, ui32SysClkFreq, baudrate,
                (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    UARTIntRegister(UART3_BASE,UART3IntHandler);
    UARTIntEnable(UART3_BASE,UART_INT_RX);
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

size_t uart_get_n(void * src_ptr, size_t len)
{
    unsigned char * temp;
    int i;

    temp= (unsigned char *)src_ptr;

    for(i=0;i<len;i++)
    {
        *(temp+i)=(char)UARTCharGet(DATA_UART);
    }

    return len;
}

uint32_t length;
packet_data_t my_packet;

void UART3IntHandler(void)
{
    UARTIntClear(UART3_BASE,UART_INT_RX);

    UARTIntDisable(UART3_BASE,UART_INT_RX);
    //if (UARTCharsAvail(UART3_BASE)) UARTCharPut(UART0_BASE, UARTCharGet(UART3_BASE));

    // Move this part to a delayed interrupt handler task
    while (UARTCharsAvail(UART3_BASE))
    {

        if(UARTCharGet(UART3_BASE)==0xFE)
        {
            LEDON(LED1); // UART receive activity
            LEDOFF(LED2);
            uart_get_n(&length,4);
            uart_get_n(&my_packet,(length-4));

            //UARTprintf("ts: %d",my_packet.header.timestamp);
            print_data_packet(&my_packet);
        }
        else
        {
            LEDOFF(LED1);
            LEDON(LED2); //looking for magic character
        }

    }

    UARTIntEnable(UART3_BASE,UART_INT_RX);
    LEDOFF(LED1);
    LEDOFF(LED2);
}



/*
 *     UARTprintf("%d\n",IntPriorityGet(INT_UART0));
    UARTprintf("%d\n",IntPriorityGet(INT_UART3));
    UARTprintf("%d\n",IntPriorityGet(INT_EMAC0));
    UARTprintf("%d\n",IntPriorityGet(INT_SYSCTL));
    UARTprintf("%d\n",IntPriorityGet(INT_SYSEXC));
 */

