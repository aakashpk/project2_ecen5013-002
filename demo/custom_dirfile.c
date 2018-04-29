/*

gcc -g custom_dirfile.c -I../common -lgetdata -lpthread

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
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <pthread.h>
#include <fcntl.h>
#include<sys/stat.h>

#include "packet_data_type.h"

// Todo - consolidate write_flush and fwrite_check
void write_flush(FILE *fp, void *data, size_t len, bool force_flush)
{
    assert(1 == fwrite(data, len, 1, fp));

    if (force_flush)
    {
        assert(0 == fflush(fp));
    }
}

FILE *fopen_check(const char *path, char *flags)
{
    FILE *fp;
    assert((fp = fopen(path, flags)));
    return fp;
}

#define PATH_LENGTH_LIMIT 256
FILE *fopen_dir_check(const char *dir, const char *name, char *flags)
{
    char path[PATH_LENGTH_LIMIT];
    strncpy(path, dir, PATH_LENGTH_LIMIT);
    strncat(path, "/", PATH_LENGTH_LIMIT);
    strncat(path, name, PATH_LENGTH_LIMIT);

    struct stat st = {0};
    if (stat(dir, &st) == -1)
    {
        mkdir(dir, 0700);
    }

    return fopen_check(path, flags);
}

size_t fwrite_check(const void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    size_t elements_written = fwrite(ptr, size, nmemb, stream);
    if (nmemb != elements_written)
    {
        printf("fwrite could not write all data\n");
    }
    return elements_written;
}

uint8_t get_checksum(uint8_t* buf, size_t len, uint8_t seed)
{
    for (size_t i = 0; i < len; i++)
    {
        seed += buf[i];
    }
    return seed;
}

typedef enum
{
    OUTPUT_TO_FILE,
    OUTPUT_TO_SERIAL,
    OUTPUT_TO_SOCKET,
    NUM_OUTPUT_MODES,
} data_output_mode_t;

typedef struct
{
    data_output_mode_t current_output_mode;

    union // output_handle
    {
        FILE *output_fp;
        // todo serial
        // *output_serial_p;
        // todo socket
        // *output_socket_p;
    };
} data_output_t;

// data output write functions
void data_output_write_file(data_output_t *output, char* data, size_t len)
{
    fwrite_check(data, len, 1, output->output_fp);
}

void data_output_write_serial(data_output_t *output, char* data, size_t len)
{
    // todo serial write
}

void data_output_write_socket(data_output_t *output, char* data, size_t len)
{
    // todo socket write
}

// data output open functions
void data_output_open_file(data_output_t *output, char* name)
{
    output->output_fp = fopen_check(name, "w");
}

void data_output_open_serial(data_output_t *output, char* name)
{
    // todo serial open
}

void data_output_open_socket(data_output_t *output, char* name)
{
    // todo socket open
}


// data output close functions
void data_output_close_file(data_output_t *output)
{
    fclose(output->output_fp);
}

void data_output_close_serial(data_output_t *output)
{
    // todo serial close
}

void data_output_close_socket(data_output_t *output)
{
    // todo socket close
}

// Should not make function pointers order dependent - should index by enum
/*
// write function pointers
void (*data_output_write_func[]) (data_output_t *output, char* data, size_t len) = [
    data_output_write_file,
    data_output_write_serial,
    data_output_write_socket
];

// open function pointers
void (*data_output_open_func[]) (data_output_t *output, char* name) = [
    data_output_open_file,
    data_output_open_serial,
    data_output_open_socket
];

// close function pointers
void (*data_output_close[]) (data_output_t *output) = [
    data_output_close_file,
    data_output_close_serial,
    data_output_close_socket
];
*/

void data_output_write(data_output_t *output, char* data, size_t len)
{
    // Todo - convert enum arrays to named initializer
    // https://eli.thegreenplace.net/2011/02/15/array-initialization-with-enum-indices-in-c-but-not-c


    // write function pointers
    void (*data_output_write_func[NUM_OUTPUT_MODES])(data_output_t *output, char *data, size_t len);
    data_output_write_func[OUTPUT_TO_FILE] = data_output_write_file;
    data_output_write_func[OUTPUT_TO_SERIAL] = data_output_write_serial;
    data_output_write_func[OUTPUT_TO_SOCKET] = data_output_write_socket;

    data_output_write_func[output->current_output_mode](output, data, len);
}

void data_output_open(data_output_t *output, char* name)
{
    // open function pointers
    void (*data_output_open_func[NUM_OUTPUT_MODES])(data_output_t *output, char *name);
    data_output_open_func[OUTPUT_TO_FILE] = data_output_open_file;
    data_output_open_func[OUTPUT_TO_SERIAL] = data_output_open_serial;
    data_output_open_func[OUTPUT_TO_SOCKET] = data_output_open_socket;

    data_output_open_func[output->current_output_mode](output, name);
}

void data_output_close(data_output_t *output)
{
    // close function pointers
    void (*data_output_close_func[NUM_OUTPUT_MODES])(data_output_t *output);
    data_output_close_func[OUTPUT_TO_FILE] = data_output_close_file;
    data_output_close_func[OUTPUT_TO_SERIAL] = data_output_close_serial;
    data_output_close_func[OUTPUT_TO_SOCKET] = data_output_close_socket;

    data_output_close_func[output->current_output_mode](output);
}

// Todo - may want to add flush function


/* Todo - move data_output to its own file
Just need to expose modes, handle, and top level write, open, close
Could make this more defensive if necessary

Other communication lib may manage their copies of data_output_t
*/

// This should be an extern (implicit with const) of packet_data_type.c
const uint32_t magic_num = 0xFEEDCAFE;

/*
// Out of order risk with this version
size_t packet_size[]
{
    sizeof(motor_values_t),
    sizeof(pid_param_t),
    sizeof(pid_config_t),
}
*/

static const size_t packet_payload_size[] = {
    [UNINITIALISED] = 0,
    [COMM_HEARTBEAT] = 0,
    [MOTOR_VALUES] = sizeof(motor_values_t),
    [PID_PARAMETERS] = sizeof(pid_param_t),
    [PID_CONFIGUARTION] = sizeof(pid_config_t),
};


/* Assuming caller populates all required fields,
although could absorb some common functionality here,
such as length, type, another timestamp, and checksum.
Biggest challenge is lack of timestamp when sending structs to queue.
Can optimize later, may help to learn more inner workings of freertos queues
Somewhat risky to assume valid data here.
Would also ideally like to send header and data separately to avoid padding risk.
*/

void write_packet(data_output_t *output, packet_data_t *data)
{
    size_t payload_len = packet_payload_size[data->header.packet_type];

    data_output_write(output, (char*)&magic_num, sizeof(magic_num));
    data_output_write(output, (char*)&data->header, sizeof(data->header));
    data_output_write(output, (char*)&data->payload, payload_len);

    uint8_t checksum = 0;
    checksum = get_checksum((uint8_t*)&data->header, sizeof(data->header), checksum);
    checksum = get_checksum((uint8_t*)&data->motor_values, payload_len, checksum);
    data_output_write(output, (char*)&checksum, sizeof(checksum));
}

void *datagen_task(void *ptr)
{
    data_output_t output;

    // Todo - can make data_output interactions even safer.
    // eg, auto close previous when attempting to open another output.
    output.current_output_mode = OUTPUT_TO_FILE;

    data_output_open(&output, (char*)ptr);

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

            write_packet(&output, &packet);
        }

        if (!(count % 10)) // slower update based on reconfig
        {
            packet.header.packet_type = PID_PARAMETERS;
            packet.header.timestamp = count;

            packet.pid_param.kp = 7 + base + offset;
            packet.pid_param.ki = 8 + base + offset;
            packet.pid_param.kd = 9 + base + offset;

            write_packet(&output, &packet);
        }

        if (!(count % 100)) // usually very slow update
        {
            packet.header.packet_type = PID_CONFIGUARTION;
            packet.header.timestamp = count;

            packet.pid_config.auto_tune = 10 + base + offset;
            packet.pid_config.update_period_ns = 11 + base + offset;
            packet.pid_config.windup_limit = 12 + base + offset;

            write_packet(&output, &packet);
            printf("datagen wrote config values\n");
        }

        usleep(1E4); // sleep 10ms
    }

    data_output_close(&output);

    return NULL;
}

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

    write_flush(handle->fp_speed, (void *)&motor_values->speed, 4, force_flush);
    write_flush(handle->fp_setpoint, (void *)&motor_values->setpoint, 4, force_flush);
    write_flush(handle->fp_error, (void *)&motor_values->error, 4, force_flush);

    write_flush(handle->fp_pwm_output, (void *)&motor_values->pwm_output, 4, force_flush);

    write_flush(handle->fp_p_value, (void *)&motor_values->p_value, 4, force_flush);
    write_flush(handle->fp_i_value, (void *)&motor_values->i_value, 4, force_flush);
    write_flush(handle->fp_d_value, (void *)&motor_values->d_value, 4, force_flush);

    write_flush(handle->fp_kp, (void *)&pid_param->kp, 4, force_flush);
    write_flush(handle->fp_ki, (void *)&pid_param->ki, 4, force_flush);
    write_flush(handle->fp_kd, (void *)&pid_param->kd, 4, force_flush);

    write_flush(handle->fp_auto_tune, (void *)&pid_config->auto_tune, 4, force_flush);
    write_flush(handle->fp_update_period_ns, (void *)&pid_config->update_period_ns, 4, force_flush);
    write_flush(handle->fp_windup_limit, (void *)&pid_config->windup_limit, 4, force_flush);
}

int main(void)
{
    printf("starting\n");

    // --------------- open files --------------
    char dir[] = "demo_dir";

    dir_handles_t dir_handles;
    create_dirfile(&dir_handles, dir);

    // --------------------------------------------

    // These are all persistent local copies
    motor_values_t motor_values;
    pid_param_t pid_param;
    pid_config_t pid_config;

    char input_path[] = "combo_structs";

#define GEN_ENABLE 1
#define GEN_BLOCK 1
#if GEN_ENABLE
    // datagen for testing
    pthread_t datagen_thread;
    int retval;

    if ((retval = pthread_create(&datagen_thread, NULL, datagen_task, (void *)input_path)))
    {
        printf("pthread create issue %s\n", strerror(retval));
        abort();
    }
#endif
    // File read will return 0 bytes at EOF and trigger loop exit
    // This is fine for testing. Other communication schemes will support better blocking.

    sleep(1); // allow file to be created while debugging

#if GEN_ENABLE
#if GEN_BLOCK
    // Force continuous or time-limited data generation
    pthread_join(datagen_thread, NULL);
#endif
#endif
    // File read will return 0 bytes at EOF and trigger loop exit

    // Todo - will need to read command line speed configuration commands as well as log data.
    // Solution involves select on fdset
    // Or could just rely on fast logging constantly unblocking read.
    // Check for character echo rate to see if delay is an issue.
    // Only a problem if annoying to humans. Will be very annoying if motor unplugged.
    // This is difficult to test locally, since terminal is almost always buffered.
    // May need ncurses or terminos
    // https://stackoverflow.com/questions/31963835/how-to-read-terminals-input-buffer-immediately-after-keypress

    // Need to figure out how to get status logging working when ncurses controls terminal.
    // Could just replace all printf with log to status file that can be tailed in another terminal.

    int input_fd;
    if (-1 == (input_fd = open(input_path, O_RDONLY)))
    {
        perror("error opening file");
        abort();
    }

    // todo - convert read() call to wrapped version supporting all three input modes

    // Don't put this large buffer on stack
    //static const size_t RX_BUF_SIZE = 1 << 12; //4kB
    #define RX_BUF_SIZE (1 << 12) //4kB
    static unsigned char rx_buf[RX_BUF_SIZE];

    ssize_t bytes_read;
    size_t rx_buf_bytes = 0;
    // Read() is blocking
    // Zero bytes read means eof / pipe closed by writer
    // Appends to partially filled buffer to account for partial packet writes / reads
    while ((bytes_read = read(input_fd, rx_buf + rx_buf_bytes, RX_BUF_SIZE - rx_buf_bytes)))
    {
        if (bytes_read == -1)
        {
            perror("error reading pipe");
            return -1;
        }

/*
        printf("read %u bytes\n", bytes_read);
        for (int i = 0; i < bytes_read; i++)
        {
            printf("0x%x\n", rx_buf[i]);
        }
        printf("read %u bytes\n", bytes_read);
*/

        rx_buf_bytes += bytes_read;

        size_t bytes_consumed = 0;
        size_t bytes_remaining = 0;

        // Don't need to worry about advanced re-sync with fixed-size packets
        static bool out_of_sync = true;

        // Process all messages
        // Make sure we can get at least the minimum sized message
        while ((bytes_remaining = rx_buf_bytes - bytes_consumed) >=
               (sizeof(magic_num) + sizeof(packet_header_t)))
        {
            // Check for magic number
            if (magic_num != *(uint32_t*)(rx_buf + bytes_consumed))
            {
                if (!out_of_sync)
                {
                    printf("lost sync (no magic num)");
                }
                printf(".");
                out_of_sync = true;
                bytes_consumed++;
                continue;
            }
            else
            {
                if (out_of_sync)
                {
                    printf("regained sync\n");
                }
                out_of_sync = false;
            }

            // latter code starts parsing after magic num
            bytes_consumed += sizeof(magic_num);
            bytes_remaining -= sizeof(magic_num);


            packet_data_t *packet = (packet_data_t*)(rx_buf + bytes_consumed);
            packet_type_t packet_type = packet->header.packet_type;

            // error check for correct packet type
            // Doing this early, so we can use common checksum code
            if (packet_type >= NUM_PACKET_TYPES)
            {
                // out of sync
                // re-scan for magic num
                printf("lost sync (bad type %d)", packet_type);
                out_of_sync = true;
                continue;
            }

            uint8_t checksum = 0;

            // error check for large enough packet
            size_t required_packet_size = sizeof(packet_header_t) +
                                          packet_payload_size[packet_type] +
                                          sizeof(checksum);

            // bytes_consumed advanced earlier, but
            if (bytes_remaining < required_packet_size)
            {
                // not enough bytes
                printf("not enough bytes for packet type %d. Required %zu, available %zu\n",
                       packet_type, required_packet_size, bytes_remaining);
                break;
            }

            // verify checksum
            checksum = get_checksum((uint8_t*)&packet->header, sizeof(packet->header), 0);
            checksum = get_checksum((uint8_t*)&packet->payload, packet_payload_size[packet_type], checksum);

            uint8_t embedded_checksum = *(uint8_t*)(rx_buf + bytes_consumed + required_packet_size - sizeof(checksum));
            if (checksum != embedded_checksum)
            {
                // out of sync
                // re-scan for magic num
                printf("lost sync (checksum mismatch %u != %u embedded)", checksum, embedded_checksum);
                out_of_sync = true;
                continue;
            }

            switch (packet_type)
            {
                default:
                {
                    printf("lost sync (packet type %u uninitialized or invalid)", packet_type);
                    out_of_sync = true;
                    continue;
                    break;
                }
                case COMM_HEARTBEAT:
                {
                    printf("got heartbeat, timestamp %u\n", packet->header.timestamp);
                    bytes_consumed += required_packet_size;
                    break;
                }
                case PID_CONFIGUARTION:
                {
                    memcpy(&pid_config, &packet->pid_config, sizeof(pid_config));

                    printf("wrote pid config, windup_limit %f\n", pid_config.windup_limit);

                    bytes_consumed += required_packet_size;
                    break;
                }
                case PID_PARAMETERS:
                {
                    memcpy(&pid_param, &packet->pid_param, sizeof(pid_param));

                    printf("wrote pid param, kp %f\n", pid_param.kp);

                    bytes_consumed += required_packet_size;
                    break;
                }
                case MOTOR_VALUES:
                {
                    // This memcpy is redundant, but enables some flexibility
                    memcpy(&motor_values, &packet->motor_values, sizeof(motor_values));

                    printf("wrote motor values, speed %f\n", motor_values.speed);

                    //bytes_consumed += sizeof(packet->header) + sizeof(packet->motor_values) + sizeof(checksum);
                    // same as above
                    bytes_consumed += required_packet_size;

                    // Todo - how to deal with synchronizing data across files?
                    // Simple solution is to just do all writes in here.
                    // This is one of the non-ideal aspects of dirfile timeseries, which wouldn't be an issue in custom plotter
                    // Must write within packet processing loop to handle multipe incoming motor packets per read cycle.

                    // Last parameter is flush interval
                    write_dirfile_entry(&dir_handles, &motor_values, &pid_param, &pid_config, 10);

                    // To simulate continuous file read while debugging
                    usleep(1E4);

                    break;
                }
            }
        }

        // Eliminate consumed bytes
        rx_buf_bytes -= bytes_consumed;

        // Shift remaining bytes to beginning of buffer if necessary
        if (rx_buf_bytes)
        {
            memmove(rx_buf, rx_buf + bytes_consumed, rx_buf_bytes);
        }
    }

    printf("read zero bytes (EOF) - main loop exiting\n");

    close_dirfile(&dir_handles);

#if GEN_ENABLE
    pthread_join(datagen_thread, NULL);
#endif

    return 0;
}
