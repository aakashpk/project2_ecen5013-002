#include "bbb_packet_handling.h"

#include <stdlib.h> // abort
#include <string.h> // memcpy
#include <unistd.h> // usleep

#include "packet_parser.h"
#include "file_helper.h" // fopen_check

// regular open
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define IO_SCHEME FILE
// pthread
void *bbb_packet_parsing_task(void* ptr)
{
    bbb_packet_parsing_task_params_t *param = (bbb_packet_parsing_task_params_t *)ptr;

#if (IO_SCHEME == FILE)

    int input_fd = open(param->input_path, O_RDONLY);

#if 0
    // Open input file
    FILE *input_fp = fopen_check(param->input_path, "r");
    // Disable buffering
    setvbuf(input_fp, NULL, _IONBF, 0);
#endif

#elif (IO_SCHEME == UART)

#if (PLATFORM == HOST)
#elif (PLATFORM == BBB)
#endif // PLATFORM


#endif // IO_SCHEME

    // Open dirfile output
    dir_handles_t dirfile_handles;
    create_dirfile(&dirfile_handles, param->output_dirfile_path);

    // Create buffer, but don't put stack
    //static const size_t buffer_size = 1 << 12; //4kB
    // Question: Why doesn't the above work
    #define BUF_SIZE (1 << 12) //4kB
    static uint8_t buffer[BUF_SIZE];

    // Prepare additional callback params
    file_wrapper_params_t file_wrapper_params;
    //file_wrapper_params.fp = input_fp;
    file_wrapper_params.fd = input_fd;

    bbb_packet_handler_params_t bbb_packet_handler_params;
    bbb_packet_handler_params.dirfile_handles = &dirfile_handles;

    // call packet parsing function, which runs forever
    packet_parser(buffer, BUF_SIZE,
                  fread_wrapper_callback, &file_wrapper_params,
                  bbb_packet_handler_callback, &bbb_packet_handler_params);

    // packet parser somehow returned, so should probably close files

    close(input_fd);
    //fclose(input_fp);


    close_dirfile(&dirfile_handles);

    return NULL;
}

size_t fread_wrapper_callback(void *buffer, size_t len, void *additional_params)
{
    file_wrapper_params_t *param = (file_wrapper_params_t*)additional_params;
    //size_t bytes_read = fread(buffer, 1, len, param->fp);
    size_t bytes_read = read(param->fd, buffer, len);

    if (bytes_read == -1)
    {
        perror("error reading file");
        abort();
    }

    return bytes_read;
}

void fwrite_wrapper_callback(void *buffer, size_t len, void *additional_params)
{
    file_wrapper_params_t *param = (file_wrapper_params_t*)additional_params;
    fwrite_flush(param->fp, buffer, len, 0); // no flush here
}

void fflush_wrapper_callback(void *additional_params)
{
    file_wrapper_params_t *param = (file_wrapper_params_t*)additional_params;
    assert(0 == fflush(param->fp));
}

void bbb_packet_handler_callback(packet_data_t *packet, void* additional_params)
{
    bbb_packet_handler_params_t *param = (bbb_packet_handler_params_t *)additional_params;

    /*
    Dirfile timeseries requires all fields share same time base,
    So writing all data out to match most frequently updated fields,
    which is motor values.
    Saving copies of other values for dirfile output later
    */

    // Persistent local copies
    static pid_param_t pid_param = {0};
    static pid_config_t pid_config = {0};

    switch (packet->header.packet_type)
    {
    default:
    {
        printf("uninitialized or invalid packet type %u\n", packet->header.packet_type);
        break;
    }
    case COMM_HEARTBEAT:
    {
        printf("got heartbeat, timestamp %u\n", packet->header.timestamp);
        break;
    }
    case PID_CONFIGUARTION:
    {
        memcpy(&pid_config, &packet->pid_config, sizeof(pid_config));
        printf("wrote pid config, windup_limit %f\n", pid_config.windup_limit);
        break;
    }
    case PID_PARAMETERS:
    {
        memcpy(&pid_param, &packet->pid_param, sizeof(pid_param));
        printf("wrote pid param, kp %f\n", pid_param.kp);
        break;
    }
    case MOTOR_VALUES:
    {
        #if 0
        /*
            This version causes bus error due to un-alligned access.
            For example, canot read float if not on word boundary.
            Tried reproducing this problem in smaller bus_error_allignment_test.c,
            but compiler seems to find ways to avoid the problem in simpler program.
        */
        motor_values_t *motor_values = &packet->motor_values;
        printf("wrote motor values, speed %f\n", motor_values->speed);
        // Last parameter is flush interval
        write_dirfile_entry(param->dirfile_handles, motor_values, &pid_param, &pid_config, 10);
        #else
        // No bus error, but inefficient memcpy
        motor_values_t motor_values;
        memcpy(&motor_values, &packet->motor_values, sizeof(motor_values));
        printf("wrote motor values, speed %f\n", motor_values.speed);
        // Last parameter is flush interval
        write_dirfile_entry(param->dirfile_handles, &motor_values, &pid_param, &pid_config, 10);
        #endif

        // To simulate continuous file read while debugging
        usleep(1E4);

        break;
    }
    }
}
