/*
 * ethernet.h
 *
 *  Created on: Apr 22, 2018
 *      Author: aakash
 */

#ifndef DRIVER_ETHERNET_H_
#define DRIVER_ETHERNET_H_

#include <stdint.h>
#include <stdbool.h>

/* Hardware includes. */
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_nvic.h"
#include "driverlib/emac.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/flash.h"
#include "driverlib/interrupt.h"

#include "driver/serial_interface.h"

extern uint32_t  g_ui32SysClock;

void enable_eth0(void);

int32_t ProcessReceivedPacket(void);
void InitDescriptors(uint32_t ui32Base);


//BaseType_t vSendPing( const char *pcIPAddress );

#endif /* DRIVER_ETHERNET_H_ */
