#include "tiva_packet_handling.h"

#include <stdlib.h> // abort
#include <string.h> // memcpy
//#include <unistd.h> // usleep

#include "packet_parser.h"
#include "project2_tasks.h"

// pthread
void *tiva_packet_parsing_task(void* ptr)
{
    //bbb_packet_parsing_task_params_t *param = (bbb_packet_parsing_task_params_t *)ptr;

    // Assume UART is already configured

    // Create buffer, but don't put stack
    //static const size_t buffer_size = 1 << 12; //4kB
    // Question: Why doesn't the above work
    #define BUF_SIZE (1 << 12) //4kB
    static uint8_t buffer[BUF_SIZE];

    // call packet parsing function, which runs forever
    packet_parser(buffer, BUF_SIZE,
                  tiva_uart_read_wrapper_callback, NULL,
                  tiva_packet_handler_callback, NULL);

    return NULL;
}

size_t tiva_uart_read_wrapper_callback(void *buffer, size_t len, void *additional_params)
{
    return uart_fill_buffer(buffer, len);
}

void tiva_uart_write_wrapper_callback(void *buffer, size_t len, void *additional_params)
{
    uart_send_n(buffer, len);
}

void tiva_packet_handler_callback(packet_data_t *packet, void* additional_params)
{
    //tiva_packet_handler_params_t *param = (tiva_packet_handler_params_t *)additional_params;

    switch (packet->header.packet_type)
    {
    default:
    {
        //printf("uninitialized or invalid packet type %u\n", packet->header.packet_type);
        break;
    }
    case COMM_HEARTBEAT:
    {
        // Todo - might get to this later
        //printf("got heartbeat, timestamp %u\n", packet->header.timestamp);
        break;
    }
    case PID_CONFIGUARTION:
    {
        // Todo
        //memcpy(&pid_config, &packet->pid_config, sizeof(pid_config));
        //printf("wrote pid config, windup_limit %f\n", pid_config.windup_limit);
        break;
    }
    case PID_PARAMETERS:
    {
        // Copy to tiva global
        memcpy(&g_pid_params, &packet->pid_param, sizeof(g_pid_params));
        //printf("wrote pid param, kp %f\n", pid_param.kp);
        break;
    }
    case MOTOR_VALUES:
    {
        break;
    }
    }
}
