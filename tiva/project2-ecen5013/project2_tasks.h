//*****************************************************************************

#ifndef __MY_TASKS_H__
#define __MY_TASKS_H__

#include <stdint.h>
#include <stdbool.h>

#include "priorities.h"

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"

# include "driver/serial_interface.h"
# include "driver/leds.h"


#include "utils/uartstdio.h"

//*****************************************************************************
//
// Prototypes for the  task.
//
//*****************************************************************************

uint32_t task1Init(void);

uint32_t task2Init(void);

uint32_t task3Init(void);

#endif // __MY_TASKS_H__
