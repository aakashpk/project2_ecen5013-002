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

void create_dirfile(dir_handles_t *handle, char* dir)
{
    handle->fp_speed = fopen_dir_check(dir, "speed", "w");
    handle->fp_setpoint = fopen_dir_check(dir, "setpoint", "w");
    handle->fp_error = fopen_dir_check(dir, "error", "w");

    handle->fp_pwm_output = fopen_dir_check(dir, "pwm_output", "w");

    handle->fp_p_value = fopen_dir_check(dir, "p_value", "w");
    handle->fp_i_value = fopen_dir_check(dir, "i_value", "w");
    handle->fp_d_value = fopen_dir_check(dir, "d_value", "w");

    handle->fp_kp = fopen_dir_check(dir, "kp", "w");
    handle->fp_ki = fopen_dir_check(dir, "ki", "w");
    handle->fp_kd = fopen_dir_check(dir, "kd", "w");

    handle->fp_auto_tune = fopen_dir_check(dir, "auto_tune", "w");
    handle->fp_update_period_ns = fopen_dir_check(dir, "update_period_ns", "w");
    handle->fp_windup_limit = fopen_dir_check(dir, "windup_limit", "w");
}

void close_dirfile(dir_handles_t *handle)
{
    fclose(handle->fp_speed);
    fclose(handle->fp_setpoint);
    fclose(handle->fp_error);

    fclose(handle->fp_pwm_output);

    fclose(handle->fp_p_value);
    fclose(handle->fp_i_value);
    fclose(handle->fp_d_value);

    fclose(handle->fp_kp);
    fclose(handle->fp_ki);
    fclose(handle->fp_kd);

    fclose(handle->fp_auto_tune);
    fclose(handle->fp_update_period_ns);
    fclose(handle->fp_windup_limit);
}

void write_dirfile_entry(dir_handles_t *handle,
                         motor_values_t *motor_values,
                         pid_param_t *pid_param,
                         pid_config_t *pid_config,
                         uint32_t flush_interval)
{
    // Check if we need to force a flush
    // Longer interval could potentially enable better performance if file writes are slow
    static uint32_t unflushed_write_count = 0;
    unflushed_write_count++;
    bool force_flush = false;
    if (unflushed_write_count >= flush_interval)
    {
        force_flush = true;
        unflushed_write_count = 0;
        printf("flushing\n");
    }

    // -------------- Write all structs to files -----------------
    // could use sizeof for each field instead of magic number 4, but that gets too busy

    fwrite_flush(handle->fp_speed, (void *)&motor_values->speed, 4, force_flush);
    fwrite_flush(handle->fp_setpoint, (void *)&motor_values->setpoint, 4, force_flush);
    fwrite_flush(handle->fp_error, (void *)&motor_values->error, 4, force_flush);

    fwrite_flush(handle->fp_pwm_output, (void *)&motor_values->pwm_output, 4, force_flush);

    fwrite_flush(handle->fp_p_value, (void *)&motor_values->p_value, 4, force_flush);
    fwrite_flush(handle->fp_i_value, (void *)&motor_values->i_value, 4, force_flush);
    fwrite_flush(handle->fp_d_value, (void *)&motor_values->d_value, 4, force_flush);

    fwrite_flush(handle->fp_kp, (void *)&pid_param->kp, 4, force_flush);
    fwrite_flush(handle->fp_ki, (void *)&pid_param->ki, 4, force_flush);
    fwrite_flush(handle->fp_kd, (void *)&pid_param->kd, 4, force_flush);

    fwrite_flush(handle->fp_auto_tune, (void *)&pid_config->auto_tune, 4, force_flush);
    fwrite_flush(handle->fp_update_period_ns, (void *)&pid_config->update_period_ns, 4, force_flush);
    fwrite_flush(handle->fp_windup_limit, (void *)&pid_config->windup_limit, 4, force_flush);
}