#include "project2_tasks.h"

#define TASKSTACKSIZE        128         // Stack size in words

#define HEARTBEAT_NOTIFY (0x01)
#define LOG_NOTIFY (0x02)




static TaskHandle_t xMotor_task = NULL, xSpeed_measure_task = NULL,
        xCurrent_measure_task = NULL,xLogger_task=NULL;

TimerHandle_t xTimer1,xTimer2;

bool toggle=true;

extern QueueHandle_t xQueue1;

extern xSemaphoreHandle g_logtask_Semaphore;

motor_values_t gMotorValues;

extern SemaphoreHandle_t xData_Semaphore;

/* Timer callbacks */

void heartbeat_timer_callback(TimerHandle_t xTimer)
{
    if(xTaskNotify(xLogger_task,HEARTBEAT_NOTIFY,eSetValueWithOverwrite)!=pdTRUE)
             UARTprintf("Notification not passed from Heartbeat timer\n");
}

void logger_timer_callback(TimerHandle_t xTimer)
{
    if(xTaskNotify(xLogger_task,LOG_NOTIFY,eSetValueWithOverwrite)!=pdTRUE)
             UARTprintf("Notification not passed from Logger timer\n");

}

void current_update_timer_callback(TimerHandle_t xTimer)
{
    xTaskNotifyGive(xCurrent_measure_task);

}


void pid_timer_callback(TimerHandle_t xTimer)
{
    xTaskNotifyGive(xMotor_task);

}


/*
 * ** Task functions **
 *
 *
 * */

static void motor_task(void *pvParameters)
{
    TimerHandle_t xMotor_timer;
    xMotor_timer = xTimerCreate("Logger_Timer",pdMS_TO_TICKS(MOTOR_CALC_INTERVAL),
                                pdTRUE,(void *) 0,pid_timer_callback);
    if( xTimerStart(xMotor_timer, 0 ) != pdPASS )
        UARTprintf("Current measure timer setup Error !!\n");

    while(1)
    {
        ulTaskNotifyTake( pdTRUE, portMAX_DELAY );
        UARTprintf("---Motor Calc \n");

        // Add pid calcs here and write to motor
        /*
            motor_speed(speed);
            speed+=5;
            if(speed>95) speed=0;*/
    if(xTaskNotify(xLogger_task,LOG_NOTIFY,eSetValueWithOverwrite)!=pdTRUE)
                UARTprintf("Notification not passed from Logger timer\n");
    }

}


static void current_measure_task(void *pvParameters)
{
    TimerHandle_t xCurrent_timer;
    xCurrent_timer = xTimerCreate("Logger_Timer",pdMS_TO_TICKS(CURRENT_READ_INTERVAL),
                                pdTRUE,(void *) 0,current_update_timer_callback);
    if( xTimerStart(xCurrent_timer, 0 ) != pdPASS )
        UARTprintf("Current measure timer setup Error !!\n");
    char pstring[10];
    while(1)
    {
        ulTaskNotifyTake( pdTRUE, portMAX_DELAY );
        //xSemaphoreTake( xData_Semaphore, ( TickType_t ) 10 );
        gMotorValues.current=get_current();
        //sprintf (pstring, "%f\n", gMotorValues.current);
        //val=get_adc_count();
        UARTprintf("reading current \n");
        //xSemaphoreTake( xData_Semaphore, ( TickType_t ) 10 );

        xTaskNotifyGive(xSpeed_measure_task);
    }

}

static void speed_measure_task(void *pvParameters)
{
    uint8_t speed;



    while(1)
    {
        ulTaskNotifyTake( pdTRUE, portMAX_DELAY );
        UARTprintf("reading speed\n");
        /*
        xSemaphoreTake( xData_Semaphore, ( TickType_t ) 10 );
        gMotorValues.current=get_speed();
        xSemaphoreTake( xData_Semaphore, ( TickType_t ) 10 );
        */
        // get speed
        // lock mutex
        // write to global
        // unlock mutex
        //ulTaskNotifyTake( pdTRUE, portMAX_DELAY );
        //UARTprintf("pos: %d, speed: %d",get_position(),get_speed_test());
    }

}

static void logger_task(void *pvParameters)
{
    uint32_t event;

    packet_data_t dataPacket;

    TimerHandle_t xLog_timer,xHeartBeat_timer;

    //Create timers for heartbeat and normal logging
    xLog_timer = xTimerCreate("Logger_Timer",pdMS_TO_TICKS(LOG_INTERVAL),
                              pdTRUE,(void *) 0,logger_timer_callback);

    xHeartBeat_timer = xTimerCreate("Logger_Timer",pdMS_TO_TICKS(HEARTBEAT_INTERVAL),
                                    pdTRUE,(void *) 0,heartbeat_timer_callback);

    //Start timer and print error if failed to start
    //if( xTimerStart(xLog_timer, 0 ) != pdPASS ) UARTprintf("Logger timer Setup Error\n");
    if( xTimerStart(xHeartBeat_timer, 0 ) != pdPASS ) UARTprintf("Heartbeat timer Setup Error\n");


    while(1)
    {
        xTaskNotifyWait(0,0xffffffff,&event,portMAX_DELAY);

        if(event == HEARTBEAT_NOTIFY)
        {
            packet_send(COMM_HEARTBEAT,&dataPacket);

        }
        else if(event == LOG_NOTIFY)
        {
            packet_send(MOTOR_VALUES,&dataPacket);
            UARTprintf("Logging data\n");

        }
        else UARTprintf("[WARN] Unknown task notification\n");

    }

}


/** Task Init Functions **/

uint32_t motor_task_create(void)
{
    // Create task
    if(xTaskCreate(motor_task, (const portCHAR *)"motor_task",
                   TASKSTACKSIZE, NULL, tskIDLE_PRIORITY +
                   PRIORITY_MOTOR_TASK, &xMotor_task) != pdTRUE)
    {
        return(1); // return 1 on failure
    }

      return(0); // Successful task creation
}


uint32_t current_measure_task_create(void)
{
    // Create task
    if(xTaskCreate(current_measure_task, (const portCHAR *)"current_measure_task",
                   TASKSTACKSIZE, NULL, tskIDLE_PRIORITY +
                   PRIORITY_CURRENT_MEASURE_TASK, &xCurrent_measure_task) != pdTRUE)
    {
        return(1); // return 1 on failure
    }

      return(0); // Successful task creation
}

uint32_t speed_measure_task_create(void)
{
    // Create task
    if(xTaskCreate(speed_measure_task, (const portCHAR *)"speed_measure_task",
                   TASKSTACKSIZE, NULL, tskIDLE_PRIORITY +
                   PRIORITY_SPEED_MEASURE_TASK, &xSpeed_measure_task) != pdTRUE)
    {
        return(1); // return 1 on failure
    }

      return(0); // Successful task creation
}

uint32_t logger_task_create(void)
{
    // Create task
    if(xTaskCreate(logger_task, (const portCHAR *)"logger_task",
                   TASKSTACKSIZE, NULL, tskIDLE_PRIORITY +
                   PRIORITY_LOGGER_TASK, &xLogger_task) != pdTRUE)
    {
        return(1); // return 1 on failure
    }

      return(0); // Successful task creation
}
