//*****************************************************************************

#ifndef __PROJECT2_TASKS_H__
#define __PROJECT2_TASKS_H__

#include <stdint.h>
#include <stdbool.h>

#include "priorities.h"

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "semphr.h"

// driver headers
#include "driver/ethernet.h"
#include "driver/motor_control.h"
#include "driver/serial_interface.h"
#include "driver/leds.h"
#include "driver/current_sensor.h"

// application layer headers
#include "packet_comm.h"

// for uart printf
#include "utils/uartstdio.h"

// Time between logs in ms
#define LOG_INTERVAL 500

#define SPEED_READ_INTERVAL 10

#define CURRENT_READ_INTERVAL 100

#define HEARTBEAT_INTERVAL 1000

#define MOTOR_CALC_INTERVAL 100

//*****************************************************************************
//
// Prototypes for the  task.
//
//*****************************************************************************

uint32_t motor_task_create(void);

uint32_t speed_measure_task_create(void);

uint32_t current_measure_task_create(void);

uint32_t logger_task_create(void);

#endif // __PROJECT2_TASKS_H__
