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


#include "project2_tasks.h"
#include "NetworkInterface.h"

#include "driverlib/interrupt.h"

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

uint32_t  g_ui32SysClock;




uint8_t pui8MACAddr[6];
// Define the network addressing.  These parameters will be used if either
//ipconfigUDE_DHCP is 0 or if ipconfigUSE_DHCP is 1 but DHCP auto configuration
//failed.
/*
static const uint8_t ucIPAddress[ 4 ] = { 192, 168, 0, 200 };
static const uint8_t ucNetMask[ 4 ] = { 255, 255, 255, 0 };
static const uint8_t ucGatewayAddress[ 4 ] = { 192, 168, 0, 1 };

// The following is the address of an OpenDNS server.
static const uint8_t ucDNSServerAddress[ 4 ] = { 208, 67, 222, 222 };
*/


QueueHandle_t xQueue1,xPingReplyQueue;

xSemaphoreHandle g_logtask_Semaphore;

SemaphoreHandle_t xData_Semaphore;

int main(void)
{
    // Setup clock to run processor at 120Mhz
    g_ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                    SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |
                    SYSCTL_CFG_VCO_480), 120000000);

    //Initialize UART
    UART0_Init(g_ui32SysClock,115200);
    UART3_Init(g_ui32SysClock,115200);

    UARTprintf("UART Initialized\n");
    //
    ConfigureLEDs();

    adc_init();
    quad_encoder_init();
    pwm_init();

    motor_speed(100);

    xData_Semaphore=xSemaphoreCreateMutex();

    //g_logtask_Semaphore = xSemaphoreCreateMutex();

/*
    UARTprintf("%d\n",sizeof(packet_header_t));

    if(sizeof(packet_header_t)!=offsetof(packet_data_t,value))
        UARTprintf("True\n");
    else
        UARTprintf("Nope\n");

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




    //xQueue1=xQueueCreate(2, sizeof(TickType_t) );
*/

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
        UARTprintf("\n\n Current_measure_task not Initialized\n");

        while(1)
        {
        }
    }


    vTaskStartScheduler();

    UARTprintf("\n\nError, Scheduler returned\n");

   int i=0;

    while(1)
        {


            if(i==1000)
            {
                UARTCharPut(UART3_BASE,'A');
                i=0;
            }
            i++;
        }

}


