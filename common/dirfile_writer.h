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

void create_dirfile(dir_handles_t *handle, char* dir);

void close_dirfile(dir_handles_t *handle);

void write_dirfile_entry(dir_handles_t *handle,
                         motor_values_t *motor_values,
                         pid_param_t *pid_param,
                         pid_config_t *pid_config,
                         uint32_t flush_interval);