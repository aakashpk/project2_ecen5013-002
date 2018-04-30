#include "project2_tasks.h"

#define TASKSTACKSIZE   1024         // Stack size in words 128

#define HEARTBEAT_NOTIFY (0x01)
#define LOG_NOTIFY (0x02)


static TaskHandle_t
        xMotor_task = NULL, xSpeed_measure_task = NULL,
        xCurrent_measure_task = NULL,xLogger_task=NULL,
        xLog_receive_task=NULL;

TimerHandle_t xTimer1,xTimer2;

bool toggle=true;

extern QueueHandle_t xQueue1;

extern xSemaphoreHandle g_logtask_Semaphore;

extern SemaphoreHandle_t xData_Semaphore;


// Motor and PID globals
motor_values_t gMotorValues;
PidType g_pid_values; // from PID library
pid_param_t g_pid_params;


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
    uint32_t duty_cycle;

    Init_PID();

    gMotorValues.setpoint=50;

    TimerHandle_t xMotor_timer;
    xMotor_timer = xTimerCreate("Logger_Timer",pdMS_TO_TICKS(MOTOR_CALC_INTERVAL),
                                pdTRUE,(void *) 0,pid_timer_callback);
    if( xTimerStart(xMotor_timer, 0 ) != pdPASS )
        UARTprintf("Current measure timer setup Error !!\n");

    char fstring[20];

    while(1)
    {
        ulTaskNotifyTake( pdTRUE, portMAX_DELAY );

        duty_cycle=PID_compute_output();
        //duty_cycle+=5;
        //if(duty_cycle>95) duty_cycle=0;

        motor_speed(duty_cycle);

        UARTprintf("[LOG] PWM duty cycle %d",duty_cycle);

        sprintf (fstring, "%f", gMotorValues.setpoint);
        UARTprintf(" %s\n", fstring);


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
    char fstring[20];

    while(1)
    {
        ulTaskNotifyTake( pdTRUE, portMAX_DELAY );
        //xSemaphoreTake( xData_Semaphore, ( TickType_t ) 10 );
        gMotorValues.current=get_current();
        //xSemaphoreGive( xData_Semaphore );

        sprintf (fstring, "%f", gMotorValues.current);
        UARTprintf("[LOG] Current %s\n", fstring);

        xTaskNotifyGive(xSpeed_measure_task);
    }

}

static void speed_measure_task(void *pvParameters)
{
    char fstring[20];

    while(1)
    {
        ulTaskNotifyTake( pdTRUE, portMAX_DELAY );

        //xSemaphoreTake( xData_Semaphore, ( TickType_t ) 10 );
        gMotorValues.speed=get_speed();
        //xSemaphoreGive( xData_Semaphore );

        sprintf (fstring, "%f", gMotorValues.speed);
        UARTprintf("[LOG] Speed %s\n", fstring);

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

        }
        else UARTprintf("[WARN] Unknown task notification\n");

    }

}


static void log_receive_task(void *pvParameters)
{
    uint32_t length;
    packet_data_t my_packet;

    /* TURNS on LED1 when data is being read,
     * this LED will function as UART Rx traffic LED
     *
     * and turns on LED2 when waiting for magic
     * char, these can be used for debug externally
     *
     */
        while(1)
        {
            xSemaphoreTake( xUARTRxEventSemaphore, portMAX_DELAY );

            //if (UARTCharsAvail(UART3_BASE)) UARTCharPut(UART0_BASE, UARTCharGet(UART3_BASE));

            UARTIntDisable(UART3_BASE,UART_INT_RX);

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



uint32_t log_receive_task_create(void)
{
    // Create task
    if(xTaskCreate(log_receive_task, (const portCHAR *)"log_receive_task",
                   TASKSTACKSIZE, NULL, tskIDLE_PRIORITY +
                   PRIORITY_LOGGER_TASK, &xLog_receive_task) != pdTRUE)
    {
        return(1); // return 1 on failure
    }

      return(0); // Successful task creation
}

