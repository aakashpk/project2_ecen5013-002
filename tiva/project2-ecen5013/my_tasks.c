#include "driver/driverInit.h"
#include "priorities.h"

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"

#include "my_tasks.h"

#define TASKSTACKSIZE        128         // Stack size in words

#define TOGGLE_LED (0x01)
#define LOG_STRING (0x02)

static TaskHandle_t xTask1 = NULL, xTask2 = NULL,xTask3 = NULL;

TimerHandle_t xTimer1,xTimer2;

uint8_t ui8led1Data=2;

extern QueueHandle_t xQueue1;


void vTimer1Callback(TimerHandle_t xTimer)
{
    if(xTaskNotify(xTask3,TOGGLE_LED,eSetValueWithOverwrite)!=pdTRUE)
          UARTprintf("Notification not passed task 1\n");
}


void vTimer2Callback(TimerHandle_t xTimer)
{
    TickType_t value;
    value=xTaskGetTickCount();

    if( xQueueSend( xQueue1,( void * ) &value,( TickType_t ) 10 ) != pdPASS )
    {
        UARTprintf("Failed to write to queue\n");
    }
    if(xTaskNotify(xTask3,LOG_STRING,eSetValueWithOverwrite)!=pdTRUE)
          UARTprintf("Notification not passed task 2\n");
}

static void Task1(void *pvParameters)
{
    //Setup timer
    xTimer1 = xTimerCreate("Timer",pdMS_TO_TICKS(250), pdTRUE,(void *) 0,vTimer1Callback);

    // NULL is parameter to be passed to the callback function
    if( xTimerStart(xTimer1, 0 ) != pdPASS )
    {
        UARTprintf("Timer Setup Error\n");
    }


    while(1)
    {
        UARTprintf("-------------Task 1 Started\n");
        ulTaskNotifyTake( pdTRUE, portMAX_DELAY ); //Wait for notification from task2

        xTaskNotifyGive(xTask2); // Notify and unblock task2
    }
}


static void Task2(void *pvParameters)
{
    //Setup timer
    xTimer2 = xTimerCreate("Timer",pdMS_TO_TICKS(125), pdTRUE,(void *) 0,vTimer2Callback);

    // NULL is parameter to be passed to the callback function
    if( xTimerStart(xTimer2, 0 ) != pdPASS )
    {
        UARTprintf("Timer Setup Error\n");
    }

    while(1)
    {
        UARTprintf("-------------Task 2 Started\n");
        ulTaskNotifyTake( pdTRUE, portMAX_DELAY ); //Wait for notification from task1

        xTaskNotifyGive(xTask1); // Notify and unblock task1
    }
}


static void Task3(void *pvParameters)
{
    uint32_t event=0;
    TickType_t systick_val;

    while(1)
    {
        xTaskNotifyWait(0,0xffffffff,&event,portMAX_DELAY);

        //UARTprintf("Event Value %d\n",event);

        if(event == TOGGLE_LED)
        {
            GPIOPinWrite(GPIO_PORTN_BASE, LED1, ui8led1Data);
            ui8led1Data=~ui8led1Data;
        }
        else if(event == LOG_STRING)
        {
            // dequeue from queue and print tick
            if( xQueueReceive( xQueue1, &systick_val, ( TickType_t ) 10 ) !=pdTRUE)
            {
                UARTprintf("Failed to receive from queue\n");
            }
            UARTprintf("TASK 2 Systick value [ %d ]\n",systick_val);
        }

    }
}

uint32_t task1Init(void)
{

    //
    // Create the task1.
    //
    if(xTaskCreate(Task1, (const portCHAR *)"task1",
                   TASKSTACKSIZE, NULL, tskIDLE_PRIORITY +
                   PRIORITY_TASK1, &xTask1) != pdTRUE)
    {
        return(1);
    }

    //
    // Success.
    //
      return(0);
}



uint32_t task2Init(void)
{

    //
    // Create the task2.
    //
    if(xTaskCreate(Task2, (const portCHAR *)"task2",
                   TASKSTACKSIZE, NULL, tskIDLE_PRIORITY +
                   PRIORITY_TASK2, &xTask2) != pdTRUE)
    {
        return(1);
    }

    //
    // Success.
    //
    return(0);
}

uint32_t task3Init(void)
{

    //
    // Create the task2.
    //
    if(xTaskCreate(Task3, (const portCHAR *)"task3",
                   TASKSTACKSIZE, NULL, tskIDLE_PRIORITY +
                   PRIORITY_TASK3, &xTask3) != pdTRUE)
    {
        return(1);
    }

    //
    // Success.
    //
    return(0);
}





