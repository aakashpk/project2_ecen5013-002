/*
 * packet_comm.c
 *
 *  Created on: Apr 23, 2018
 *      Author: Aakash
 *      Author: Miles
 */

#define USEUART

#include "packet_comm.h"


#ifdef USEUART
volatile comm_hw_t comm_hw_used = UART;
#else
volatile comm_hw_t comm_hw_used = TCP;
#endif

Socket_t xSocket=0; // To be changed to extern, only to remove error now

char *packet_type_strings[] =
    { "UNINITIALISED",
      "HEARTBEAT",
      "MOTOR_VALUES",
      "PARAMETERS",
      "CONFIGUARTION",
    };


int packet_send(packet_type_t type, packet_data_t * packet)
{
    size_t length;
    switch(type)
    {
        case HEARTBEAT:
            length=get_heartbeat(packet);
            break;
        case MOTOR_VALUES:
            length=get_motor_values(packet);
            break;
        case PARAMETERS:
            length=get_pid_params(packet);
            break;
        case CONFIGUARTION:
            length=get_pid_config(packet);
            break;
        default:
            UARTprintf("WARN: Uninitialized packet send");
            return -1;
    }

    UARTprintf("Sending %s\n",packet_type_strings[type]);

    switch(comm_hw_used)
    {
        case UART:
            // magic
            // header
            // body
            // cksum
            uart_send_n((void *)packet,length);
            break;
        case TCP:
            FreeRTOS_send(xSocket,packet,length,0);
            break;
    }

    return 0;

    // TODO: -1 on failure , what else can cause a failure here ??
}

int packet_recieve( packet_data_t * packet)
{
    return 0;
}

size_t get_heartbeat(packet_data_t * packet)
{
    return sizeof(packet_header_t);
}

size_t get_motor_values(packet_data_t * packet)
{
    return (sizeof(packet_header_t)+sizeof(motor_values_t));
        //TODO: will this work properly? the struct may get resized for alignment.
        //to be checked, could do a packed struct to prevent that.
}

size_t get_pid_params(packet_data_t * packet)
{
    return (sizeof(packet_header_t)+sizeof(pid_param_t));
}


size_t get_pid_config(packet_data_t * packet)
{
    return (sizeof(packet_header_t)+sizeof(pid_config_t));
}










