/**
 * @brief 
 * 
 * @file main.c
 * @author Aakash
 * @author Miles
 * @date 2018-04-29
 */
#include <stdio.h>
#include <stddef.h>

// free RTOS headers
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "FreeRTOS_Sockets.h"
#include "FreeRTOS_IP.h"
#include "semphr.h"
#include "timers.h"

// Application layer
#include "project2_tasks.h"
#include "NetworkInterface.h"

#include "driverlib/interrupt.h"

//#define USEIP
//#define TESTIP

void vApplicationStackOverflowHook(xTaskHandle *pxTask, char *pcTaskName)
{
    //
    // This function can not return, so loop forever.  Interrupts are disabled
    // on entry to this function, so no processor interrupts will interrupt
    // this loop.
    //
    UARTprintf("\n\nSTACK OVERFLOW !!!! you messed up :( \n");
    while(1)
    {
    }
}


#ifdef USEIP
uint8_t pui8MACAddr[6];
// Define the network addressing.  These parameters will be used if either
//ipconfigUDE_DHCP is 0 or if ipconfigUSE_DHCP is 1 but DHCP auto configuration
//failed.
static const uint8_t ucIPAddress[ 4 ] = { 192, 168, 0, 200 };
static const uint8_t ucNetMask[ 4 ] = { 255, 255, 255, 0 };
static const uint8_t ucGatewayAddress[ 4 ] = { 192, 168, 0, 1 };

// The following is the address of an OpenDNS server.
static const uint8_t ucDNSServerAddress[ 4 ] = { 208, 67, 222, 222 };
#endif


uint32_t  g_ui32SysClock;
QueueHandle_t xQueue1,xPingReplyQueue;
SemaphoreHandle_t xData_Semaphore;
SemaphoreHandle_t xUARTRxEventSemaphore;


int main(void)
{
    // Setup clock to run processor at 120Mhz
    g_ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                    SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |
                    SYSCTL_CFG_VCO_480), 120000000);

    //Initialize UART
    UART0_Init(g_ui32SysClock,115200); // For console logging
    UART3_Init(g_ui32SysClock,115200); // for UART comm with BBG
    xUARTRxEventSemaphore=xSemaphoreCreateBinary();

    UARTprintf("[LOG] UART Initialized\n");


    // Initialize peripherals
    ConfigureLEDs();
    adc_init();
    quad_encoder_init();
    pwm_init();
    tachometer_init();

    //UARTprintf("\n %d \n",GPIOIntStatus(GPIO_PORTP_BASE,true));

    UARTprintf("[LOG] Peripherals Initialized\n");

    motor_speed(0);

#ifdef USEIP
    xNetworkInterfaceInitialise();

    uint32_t iptoset;

    iptoset=FreeRTOS_inet_addr_quick( ucIPAddress[ 0 ], ucIPAddress[ 1 ], ucIPAddress[ 2 ], ucIPAddress[ 3 ] );
    UARTprintf("\nRequired IP is %x\n",iptoset);

    // Initialise the TCP/IP stack.
    FreeRTOS_IPInit( ucIPAddress,ucNetMask,
                         ucGatewayAddress,
                         ucDNSServerAddress,
                         pui8MACAddr );

    xPingReplyQueue=xQueueCreate( 10,sizeof(NetworkBufferDescriptor_t));

#endif


#ifndef TESTIP

    xData_Semaphore=xSemaphoreCreateMutex();


    if(motor_task_create() != 0)
    {
        UARTprintf("\n\n Speed_measure_task not Initialized\n");

        while(1)
        {
        }
    }


    if(speed_measure_task_create() != 0)
    {
        UARTprintf("\n\n Speed_measure_task not Initialized\n");

        while(1)
        {
        }
    }


    if(current_measure_task_create() != 0)
    {
        UARTprintf("\n\n Current_measure_task not Initialized\n");

        while(1)
        {
        }
    }



    if(logger_task_create() != 0)
    {
        UARTprintf("\n\n Logger task not Initialized\n");

        while(1)
        {
        }
    }


    if(log_receive_task_create() != 0)
    {
        UARTprintf("\n\n Log RX defered ISR task not Initialized\n");

        while(1)
        {
        }
    }

#endif

    vTaskStartScheduler();

    UARTprintf("\n\nError, Scheduler returned\n");


    while(1)
        {

        }

}


