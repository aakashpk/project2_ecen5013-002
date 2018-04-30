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
        case PID_PARAMETERS:
            length=get_pid_params(packet);
            break;
        case PID_CONFIGUARTION:
            length=get_pid_config(packet);
            break;
        default:
            UARTprintf("WARN: Uninitialized packet send");
            return -1;
    }

    UARTprintf("[LOG] %s size:%d at:%d\n",packet_type_strings[type],length,packet->header.timestamp);

    static data_output_t output = {.current_output_mode=OUTPUT_TO_SERIAL};

    //write_packet(&output, packet);


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
    extern motor_values_t gMotorValues; // from packet datatype

    size_t value;
    value=(sizeof(packet_header_t)+sizeof(motor_values_t));

    packet->header.packet_type=MOTOR_VALUES;
    packet->header.timestamp=xTaskGetTickCount();
    packet->header.length=value;

    //xSemaphoreTake( xData_Semaphore, ( TickType_t ) 10 );
    packet->motor_values.speed=gMotorValues.speed;
    packet->motor_values.setpoint=gMotorValues.setpoint;
    packet->motor_values.error=gMotorValues.error;
    packet->motor_values.p_value=gMotorValues.p_value;
    packet->motor_values.i_value=gMotorValues.i_value;
    packet->motor_values.d_value=gMotorValues.d_value;
    packet->motor_values.pwm_output=gMotorValues.pwm_output;
    //xSemaphoreGive( xData_Semaphore );

    //packet->checksum=calc_checksum(MOTOR_VALUES,packet);

    return value;
}

size_t get_pid_params(packet_data_t * packet)
{
    extern pid_param_t g_pid_params;

    size_t value;
    value=(sizeof(packet_header_t)+sizeof(pid_param_t));

    packet->header.packet_type=PID_PARAMETERS;
    packet->header.timestamp=xTaskGetTickCount();
    packet->header.length=value;

    packet->pid_param.kp=g_pid_params.kp;
    packet->pid_param.ki=g_pid_params.ki;
    packet->pid_param.kd=g_pid_params.kd;

    //packet->checksum=calc_checksum(PARAMETERS,packet);

    return value;
}


size_t get_pid_config(packet_data_t * packet)
{
    size_t value;
    value=(sizeof(packet_header_t)+sizeof(pid_config_t));

    packet->header.packet_type=PID_CONFIGUARTION;
    packet->header.timestamp=xTaskGetTickCount();
    packet->header.length=value;

    //TODO: Not being implemented at this point
    packet->pid_config.auto_tune=false;
    packet->pid_config.update_period_ns=0;
    packet->pid_config.windup_limit=0;

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
        case PID_PARAMETERS:
            csm=4;
            break;
        case PID_CONFIGUARTION:
            csm=4;
            break;
    }

    return csm;
}

void print_data_packet(packet_data_t * packet)
{
    uint32_t type;

    type=packet->header.packet_type;

    switch(type)
    {
        case COMM_HEARTBEAT:
            UARTprintf("[%s] at %d\n",packet_type_strings[type],packet->header.timestamp);
            break;
        case MOTOR_VALUES:
            UARTprintf("[%s] at %d \n",packet_type_strings[type],packet->header.timestamp);
            break;
        case PID_PARAMETERS:
            UARTprintf("[%s] at %d \n",packet_type_strings[type],packet->header.timestamp);
            break;
        case PID_CONFIGUARTION:
            UARTprintf("[%s] at %d \n",packet_type_strings[type],packet->header.timestamp);
            break;
    }
}
