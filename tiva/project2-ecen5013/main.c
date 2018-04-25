#include <project2_tasks.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "FreeRTOS_sockets.h"
#include "FreeRTOS_IP.h"

#include "NetworkInterface.h"

#include "packet_comm.h"
#include <stddef.h>


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
static const uint8_t ucIPAddress[ 4 ] = { 192, 168, 0, 200 };
static const uint8_t ucNetMask[ 4 ] = { 255, 255, 255, 0 };
static const uint8_t ucGatewayAddress[ 4 ] = { 192, 168, 0, 1 };

// The following is the address of an OpenDNS server.
static const uint8_t ucDNSServerAddress[ 4 ] = { 208, 67, 222, 222 };



QueueHandle_t xQueue1,xPingReplyQueue;

int main(void)
{
    g_ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                    SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |
                    SYSCTL_CFG_VCO_480), 120000000);

    UART0_Init(g_ui32SysClock,115200);
    UART3_Init(g_ui32SysClock,115200);

    ConfigureLEDs();
/*
    typedef struct data_packet
    {
        packet_header_t header;

        // padding risk

        union
        {
            motor_values_t value;
            pid_param_t parameters;
            pid_config_t configuration;
        } ;


    }packet_data_t;

*/
    //enable_eth0();

    UARTprintf("%d\n",sizeof(packet_header_t));

    if(sizeof(packet_header_t)!=offsetof(packet_data_t,value))
        UARTprintf("True\n");
    else
        UARTprintf("Nope\n");

    xNetworkInterfaceInitialise();

    uint32_t ip;
    ip=FreeRTOS_GetIPAddress();
    UARTprintf("\n\n before %d\n",ip);

    // Initialise the TCP/IP stack.
    FreeRTOS_IPInit( ucIPAddress,ucNetMask,
                         ucGatewayAddress,
                         ucDNSServerAddress,
                         pui8MACAddr );

    ip=FreeRTOS_GetIPAddress();
    UARTprintf("\n\n IP setup %d\n",ip);

    xPingReplyQueue=xQueueCreate( 10,sizeof(NetworkBufferDescriptor_t));



    /*

    UARTCharPut(UART3_BASE, '\n');

    xQueue1=xQueueCreate(2, sizeof(TickType_t) );

    if(task1Init() != 0)
    {
        UARTprintf("\n\nTask1 not Initialized\n");

        while(1)
        {
        }
    }

   if(task2Init() != 0)
   {
       UARTprintf("\n\nTask2 not Initialized\n");

        while(1)
        {
        }
   }

   if(task3Init() != 0)
   {
       UARTprintf("\n\nTask3 not Initialized\n");

        while(1)
        {
        }
   }



    vTaskStartScheduler();

    UARTprintf("\n\nError, Scheduler returned\n");
*/
        while(1)
        {
        }

}


