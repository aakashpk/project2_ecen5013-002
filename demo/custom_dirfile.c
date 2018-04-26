

/*
No advantages to using buffered fwrite() over syscall write(),
since flush required on every write.
Hopefully OS will save contents to page cache to prevent wearing out
bbb eMMC flash, and let kds read contents more easily.
https://manybutfinite.com/post/page-cache-the-affair-between-memory-and-files/

Could have C program delete all files in dirfile directory, but that requires recursion.
Easier to just let script handle this.

Going to be somewhat tedious to repeat the following operations across all files:
open all
write
flush
close all

can setup a flush interval for performance customization

*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>

#include "packet_data_type.h"

void write_flush(FILE *fp, void *data, size_t len, uint32_t flush_period)
{
    static uint32_t call_count = 0;
    call_count++;

    assert(1 == fwrite(data, len, 1, fp));

    if (call_count >= flush_period)
    {
        assert(0 == fflush(fp));
        call_count = 0;
    }
}

FILE *fopen_check(const char *path, char *flags)
{
    FILE *fp;
    assert(fp = fopen(path, flags));
    return fp;
}

#define PATH_LENGTH_LIMIT 256
FILE *fopen_dir_check(const char *dir, const char *name, char *flags)
{
    char path[PATH_LENGTH_LIMIT];
    strncpy(path, dir, PATH_LENGTH_LIMIT);
    strncat(path, "/", PATH_LENGTH_LIMIT);
    strncat(path, name, PATH_LENGTH_LIMIT);

    return fopen_check(path, flags);
}

size_t fwrite_check(const void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    if (nmemb != fwrite(ptr, size, nmemb, stream))
    {
        printf("fwrite could not write all data\n");
    }
}

void *datagen_task(void *ptr)
{
    FILE *output_fp = fopen_check((char*)ptr, "w");

    motor_values_t motor_values;
    pid_param_t pid_param;
    pid_config_t pid_config;

    float base = 50.0;
    float offset = 0;
    srand(0); // Use same seed for reproducablility

    while (1)
    {
        // Randomly adjust plot values +/- 0.5
        float adjustment = rand();
        adjustment /= adjustment;
        adjustment -= 0.5;
        offset += adjustment;

        static uint32_t count = -1; // ensure start at zero
        count++;

        if (!(count % 1))
        {
            motor_values.speed = 0 + base + offset;
            motor_values.setpoint = 1 + base + offset;
            motor_values.error = 2 + base + offset;
            motor_values.pwm_output = 3 + base + offset;
            motor_values.p_value = 4 + base + offset;
            motor_values.i_value = 5 + base + offset;
            motor_values.d_value = 6 + base + offset;

            /*
Todo - call into generic packet wrapper function which handles arrangement of packet overhead.
Generic packet wrapper accepts function pointer to core write function, which can be swapped between
file, uart, socket.

Official logger should check each queue for updated contents, then call packet wrapper function
*/
            fwrite_check(&motor_values, sizeof(motor_values), 1, output_fp);
        }

        if (!(count % 10)) // slower update based on reconfig
        {
            pid_param.kp = 7 + base + offset;
            pid_param.ki = 8 + base + offset;
            pid_param.kd = 9 + base + offset;

            fwrite_check(&pid_param, sizeof(pid_param), 1, output_fp);
        }

        if (!(count % 100)) // usually very slow update
        {
            pid_config.auto_tune = 10 + base + offset;
            pid_config.update_period_ns = 11 + base + offset;
            pid_config.windup_limit = 12 + base + offset;

            fwrite_check(&pid_config, sizeof(pid_config), 1, output_fp);
        }
    }

    fclose(output_fp);

}

int main(void)
{
    // --------------- open files --------------
    char dir[] = "demo_dir";

    FILE *fp_speed = fopen_dir_check(dir, "speed", "w");
    FILE *fp_setpoint = fopen_dir_check(dir, "setpoint", "w");
    FILE *fp_error = fopen_dir_check(dir, "error", "w");

    FILE *fp_pwm_output = fopen_dir_check(dir, "pwm_output", "w");

    FILE *fp_p_value = fopen_dir_check(dir, "p_value", "w");
    FILE *fp_i_value = fopen_dir_check(dir, "i_value", "w");
    FILE *fp_d_value = fopen_dir_check(dir, "d_value", "w");

    FILE *fp_kp = fopen_dir_check(dir, "kp", "w");
    FILE *fp_ki = fopen_dir_check(dir, "ki", "w");
    FILE *fp_kd = fopen_dir_check(dir, "kd", "w");

    FILE *fp_auto_tune = fopen_dir_check(dir, "auto_tune", "w");
    FILE *fp_update_period_ns = fopen_dir_check(dir, "update_period_ns", "w");
    FILE *fp_windup_limit = fopen_dir_check(dir, "windup_limit", "w");

    // --------------------------------------------

    // These are all persistent local copies
    motor_values_t motor_values;
    pid_param_t pid_param;
    pid_config_t pid_config;

    char input_path[] = "combo_structs";

    // datagen for testing
    pthread_t datagen_thread;
    int retval;

    if (retval = pthread_create(&datagen_thread, NULL, datagen_task, (void *)input_path))
    {
        printf("pthread create issue %s\n", strerror(retval));
        abort();
    }

    // Todo - add file parsing code here
    // Should be configurable between reading from file, serial, sockets

    // copy data in structs other than motor_values(fastest updating) to local copy,
    // since these won't update as frequently, but still need to logged in timeseries.
    // This is one of the non-ideal aspects of dirfile timeseries, which wouldn't be an issue in custom plotter

    uint32_t flush_period = 1; // flush every write, but can slow this down for better performance

    // -------------- Write all structs to files -----------------
    // could use sizeof for each field instead of magic number 4, but that gets too busy

    write_flush(fp_speed, (void*)&motor_values.speed, 4, flush_period);
    write_flush(fp_setpoint, (void*)&motor_values.setpoint, 4, flush_period);
    write_flush(fp_error, (void*)&motor_values.error, 4, flush_period);

    write_flush(fp_pwm_output, (void*)&motor_values.pwm_output, 4, flush_period);

    write_flush(fp_p_value, (void*)&motor_values.p_value, 4, flush_period);
    write_flush(fp_i_value, (void*)&motor_values.i_value, 4, flush_period);
    write_flush(fp_d_value, (void*)&motor_values.d_value, 4, flush_period);

    write_flush(fp_kp, (void*)&pid_param.kp, 4, flush_period);
    write_flush(fp_ki, (void*)&pid_param.ki, 4, flush_period);
    write_flush(fp_kd, (void*)&pid_param.kd, 4, flush_period);

    write_flush(fp_auto_tune, (void*)&pid_config.auto_tune, 4, flush_period);
    write_flush(fp_update_period_ns, (void*)&pid_config.update_period_ns, 4, flush_period);
    write_flush(fp_windup_limit, (void*)&pid_config.windup_limit, 4, flush_period);

    // --------------------------------------------

    return 0;
}

/*
# measured from tachometer
speed RAW FLOAT32 1
# desired speed in RPS
setpoint RAW FLOAT32 1
# measured - desired
error RAW FLOAT32 1

# sent to motor
pwm_output RAW UINT32 1

# calculated terms
p_value RAW FLOAT32 1
i_value RAW FLOAT32 1
d_value RAW FLOAT32 1

# pid parameters
# these updates are not sent as frequently,
# so latest must be maintained by c-struct to dirfile converter
# to be re-written to dirfile every frame
kp RAW FLOAT32 1
ki RAW FLOAT32 1
kd RAW FLOAT32 1


# Below fields do not update often

# auto tune enabled
# bool, but can allow faster writes (in exchange for disk space),
# by writing 32-bit word.
# writing bits is also allowed, but may not be worth slowdown,
since flushing every write. Would be more ideal if flushing after many samples.
auto_tune RAW UINT32 1

# period of PID loop (time between updates),
# not the same as time interval between logged samples.
# Units of ns, which can handle period of up to 4 seconds
update_period_ns RAW UINT32 1

# maximum absolute limit of integration term
windup_limit RAW FLOAT32 1
*/
