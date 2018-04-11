#include "driver/driverInit.h"
#include "my_tasks.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


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


QueueHandle_t xQueue1;

int main(void)
{
    g_ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                    SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |
                    SYSCTL_CFG_VCO_480), 120000000);

    ConfigureUART(g_ui32SysClock);
    ConfigureLED();

    UARTprintf("\n\nAPES HW5 Q3, Hardware Init Done\n");

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

        while(1)
        {
        }

}
