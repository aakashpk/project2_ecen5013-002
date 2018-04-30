/**
 * @brief 
 * 
 * @file packet_comms.c
 * @author Aakash
 * @author Miles
 * @date 2018-04-28
 */
#include "packet_comms.h"

size_t get_heartbeat(packet_data_t * packet)
{
    size_t value;
    value=sizeof(packet_header_t);

    packet->header.packet_type=COMM_HEARTBEAT;
    packet->header.timestamp=time(NULL);
    packet->header.length=value;

    //packet->checksum=calc_checksum(COMM_HEARTBEAT,packet);

    return value;
}

size_t get_motor_values(packet_data_t * packet)
{
    size_t value;
    value=(sizeof(packet_header_t)+sizeof(motor_values_t));

    packet->header.packet_type=MOTOR_VALUES;
    packet->header.timestamp=time(NULL);
    packet->header.length=value;

    //TODO: to call correct functions here
    packet->value.speed=0;
    packet->value.setpoint=0;
    packet->value.error=0;
    packet->value.p_value=0;
    packet->value.i_value=0;
    packet->value.d_value=0;
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
    packet->header.timestamp=time(NULL);
    packet->header.length=value;

    //TODO: to call correct functions here
    packet->parameters.kp=0;
    packet->parameters.ki=0;
    packet->parameters.kd=0;

    //packet->checksum=calc_checksum(PARAMETERS,packet);

    return value;
}


size_t get_pid_config(packet_data_t * packet)
{
    size_t value;
    value=(sizeof(packet_header_t)+sizeof(pid_config_t));

    packet->header.packet_type=CONFIGUARTION;
    packet->header.timestamp=time(NULL);
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
            printf("[%s] at %d size: %d\n",packet_type_strings[type],
                                        packet->header.timestamp,packet->header.length);
            break;
        case MOTOR_VALUES:
            printf("[%s] at %d \n",packet_type_strings[type],packet->header.timestamp);
            break;
        case PARAMETERS:
            printf("[%s] at %d \n",packet_type_strings[type],packet->header.timestamp);
            break;
        case CONFIGUARTION:
            printf("[%s] at %d \n",packet_type_strings[type],packet->header.timestamp);
            break;
    }
}
