#include "packet_parser.h"
#include "dirfile_writer.h"
#include "utilities.h"
#include "data_output.h"
#include "file_helper.h"

//#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void *packet_parser_task(void* ptr)
{
    packet_parser_params_t *param = (packet_parser_params_t *)ptr;

/*
    // Not ready for this yet
    data_output_t output; // should actually be input
    output.current_output_mode = OUTPUT_TO_FILE;
*/

    //data_ouput_open()
    //size_t data_output_read(data_output_t *output, char* data, size_t len);

    // try with fopen check first
    FILE *input_fp = fopen_check(param->input_path, "r");

/*
    // Todo - convert open() and read() to support all modes (serial and socket)
    int input_fd;
    if (-1 == (input_fd = open(param->input_path, O_RDONLY)))
    {
        perror("error opening file");
        abort();
    }
*/

    // Open dirfile output
    dir_handles_t dir_handles;
    create_dirfile(&dir_handles, param->output_dir);

    // Don't put this large buffer on stack
    //static const size_t RX_BUF_SIZE = 1 << 12; //4kB
    #define RX_BUF_SIZE (1 << 12) //4kB
    static unsigned char rx_buf[RX_BUF_SIZE];

    size_t bytes_read;
    size_t rx_buf_bytes = 0;
    // Read() is blocking
    // Zero bytes read means eof / pipe closed by writer
    // Appends to partially filled buffer to account for partial packet writes / reads
    //while ((bytes_read = read(input_fd, rx_buf + rx_buf_bytes, RX_BUF_SIZE - rx_buf_bytes)))
    //fread(ptr, size, n, stream)
    while ((bytes_read = fread(rx_buf + rx_buf_bytes, 1, RX_BUF_SIZE - rx_buf_bytes, input_fp)))
    {
        if (bytes_read == -1)
        {
            perror("error reading pipe");
            abort();
        }

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

            // Error check for large enough packet.
            // Magic num not considered here, since
            // bytes_consumed and bytes_remaining advanced earlier
            size_t required_packet_size = sizeof(packet_header_t) +
                                          packet_payload_size[packet_type] +
                                          sizeof(checksum);

            if (bytes_remaining < required_packet_size)
            {
                // not enough bytes
                // No need to print this as error, will resume with rest of packet later
                //printf("not enough bytes for packet type %d. Required %zu, available %zu\n",
                //       packet_type, required_packet_size, bytes_remaining);

                // Need to undo consuming magic_num bytes for re-read later - kinda hacky
                bytes_consumed -= sizeof(magic_num);
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

            // Persistent local copies
            static motor_values_t motor_values = {0}; // This struct not necessary with current implementation
            static pid_param_t pid_param = {0};
            static pid_config_t pid_config = {0};

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

    //close(input_fd);
    fclose(input_fp);

    close_dirfile(&dir_handles);

    return NULL;
}