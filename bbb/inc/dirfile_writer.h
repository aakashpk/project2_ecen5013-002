/**
 * @brief 
 * 
 * @file dirfile_writer.h
 * @author Aakash
 * @author Miles
 * @date 2018-04-29
 */
#pragma once

#include <stdio.h>
#include "packet_data_type.h"


typedef struct
{
    FILE *fp_speed;
    FILE *fp_setpoint;
    FILE *fp_error;

    FILE *fp_pwm_output;

    FILE *fp_p_value;
    FILE *fp_i_value;
    FILE *fp_d_value;

    FILE *fp_kp;
    FILE *fp_ki;
    FILE *fp_kd;

    FILE *fp_auto_tune;
    FILE *fp_update_period_ns;
    FILE *fp_windup_limit;
} dir_handles_t;

/**
 * @brief Create a dirfile object
 * 
 * @param handle 
 * @param dir 
 */
void create_dirfile(dir_handles_t *handle, char* dir);

/**
 * @brief 
 * 
 * @param handle 
 */
void close_dirfile(dir_handles_t *handle);

/**
 * @brief 
 * 
 * @param handle 
 * @param motor_values 
 * @param pid_param 
 * @param pid_config 
 * @param flush_interval 
 */
void write_dirfile_entry(dir_handles_t *handle,
                         motor_values_t *motor_values,
                         pid_param_t *pid_param,
                         pid_config_t *pid_config,
                         uint32_t flush_interval);