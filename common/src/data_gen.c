/**
 * @brief Generates data stream
 *
 * @file data_gen.c
 * @author Aakash Kumar
 * @author Miles Frain
 * @date 2018-04-29
 */
#include "packet_writer.h"
#include "bbb_packet_handling.h"
#include "file_helper.h"
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

void *datagen_task(void *ptr)
{
    file_wrapper_params_t file_wrapper_params;
    file_wrapper_params.fp = fopen_check((char*)ptr, "w");

    packet_data_t packet;
    //packet_header_t header;
    //motor_values_t motor_values;
    //pid_param_t pid_param;
    //pid_config_t pid_config;

    float base = 50.0;
    float offset = 0;
    srand(0); // Use same seed for reproducablility

    while (1)
    {
        // Randomly adjust plot values +/- 0.5
        float adjustment = rand();
        adjustment /= INT_MAX;
        adjustment -= 0.5;
        offset += adjustment;

        static uint32_t count = -1; // ensure start at zero
        count++;

        //if (count == 10) break;

        if (!(count % 1))
        {
            packet.header.packet_type = MOTOR_VALUES;
            packet.header.timestamp = count;

            packet.motor_values.speed = 0 + base + offset;
            packet.motor_values.setpoint = 1 + base + offset;
            packet.motor_values.error = 2 + base + offset;
            packet.motor_values.pwm_output = 3 + base + offset;
            packet.motor_values.p_value = 4 + base + offset;
            packet.motor_values.i_value = 5 + base + offset;
            packet.motor_values.d_value = 6 + base + offset;

            write_packet(&packet, fwrite_wrapper_callback, fflush_wrapper_callback, &file_wrapper_params);
        }

        if (!(count % 10)) // slower update based on reconfig
        {
            packet.header.packet_type = PID_PARAMETERS;
            packet.header.timestamp = count;

            packet.pid_param.kp = 7 + base + offset;
            packet.pid_param.ki = 8 + base + offset;
            packet.pid_param.kd = 9 + base + offset;

            write_packet(&packet, fwrite_wrapper_callback, fflush_wrapper_callback, &file_wrapper_params);
        }

        if (!(count % 100)) // usually very slow update
        {
            packet.header.packet_type = PID_CONFIGUARTION;
            packet.header.timestamp = count;

            packet.pid_config.auto_tune = 10 + base + offset;
            packet.pid_config.update_period_ns = 11 + base + offset;
            packet.pid_config.windup_limit = 12 + base + offset;

            write_packet(&packet, fwrite_wrapper_callback, fflush_wrapper_callback, &file_wrapper_params);
            printf("datagen wrote config values\n");
        }

        usleep(1E4); // sleep 10ms
    }

    fclose(file_wrapper_params.fp);

    return NULL;
}