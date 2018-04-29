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
    UARTCharPut(DATA_UART, 0xFE);
    size_t length;
    switch(type)
    {
        case COMM_HEARTBEAT:
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

    UARTprintf("[LOG] %s size:%d at:%d\n",packet_type_strings[type],length,packet->header.timestamp);

    switch(comm_hw_used)
    {
        case UART:
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
    size_t value;
    value=sizeof(packet_header_t);

    packet->header.packet_type=COMM_HEARTBEAT;
    packet->header.timestamp=xTaskGetTickCount();
    packet->header.length=value;

    //packet->checksum=calc_checksum(COMM_HEARTBEAT,packet);

    return value;
}

size_t get_motor_values(packet_data_t * packet)
{
    size_t value;
    value=(sizeof(packet_header_t)+sizeof(motor_values_t));

    packet->header.packet_type=MOTOR_VALUES;
    packet->header.timestamp=xTaskGetTickCount();
    packet->header.length=value;

    //TODO: to call correct functions here
    packet->value.speed=get_speed();
    packet->value.setpoint=get_speed_setpoint();
    packet->value.error=get_error();
    packet->value.p_value=get_pvalue();
    packet->value.i_value=get_ivalue();
    packet->value.d_value=get_dvalue();
    packet->value.pwm_output=0;

    //packet->checksum=calc_checksum(MOTOR_VALUES,packet);

    return value;
        //TODO: will this work properly? the struct may get resized for alignment.
        //to be checked, could do a packed struct to prevent that.
}

size_t get_pid_params(packet_data_t * packet)
{
    size_t value;
    value=(sizeof(packet_header_t)+sizeof(pid_param_t));

    packet->header.packet_type=PARAMETERS;
    packet->header.timestamp=xTaskGetTickCount();
    packet->header.length=value;

    packet->parameters.kp=get_kp();
    packet->parameters.ki=get_ki();
    packet->parameters.kd=get_kd();

    //packet->checksum=calc_checksum(PARAMETERS,packet);

    return value;
}


size_t get_pid_config(packet_data_t * packet)
{
    size_t value;
    value=(sizeof(packet_header_t)+sizeof(pid_config_t));

    packet->header.packet_type=CONFIGUARTION;
    packet->header.timestamp=xTaskGetTickCount();
    packet->header.length=value;

    //TODO: to call correct functions here
    packet->configuration.auto_tune=false;
    packet->configuration.rate_s=0;
    packet->configuration.windup_limit=0;

    //packet->checksum=calc_checksum(CONFIGUARTION,packet);

    return value;
}


size_t calc_checksum(packet_type_t type, packet_data_t * packet)
{
    size_t csm;

    switch(type)
    {
        //TODO: calculate proper checksum values
        case COMM_HEARTBEAT:
            csm=2;
            break;
        case MOTOR_VALUES:
            csm=3;
            break;
        case PARAMETERS:
            csm=4;
            break;
        case CONFIGUARTION:
            csm=4;
            break;
    }

    return csm;
}

void print_data_packet(packet_data_t * packet)
{
    packet_type_t type;

    type=packet->header.packet_type;

    switch(type)
    {
        case COMM_HEARTBEAT:
            UARTprintf("[%s] at %d\n",packet_type_strings[type],packet->header.timestamp);
            break;
        case MOTOR_VALUES:
            UARTprintf("[%s] at %d \n",packet_type_strings[type],packet->header.timestamp);
            break;
        case PARAMETERS:
            UARTprintf("[%s] at %d \n",packet_type_strings[type],packet->header.timestamp);
            break;
        case CONFIGUARTION:
            UARTprintf("[%s] at %d \n",packet_type_strings[type],packet->header.timestamp);
            break;
    }
}
