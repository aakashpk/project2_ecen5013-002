#include "packet_parser.h"

#include <stdio.h> // perror
#include <stdlib.h> // abort
#include <string.h> // memmove

#include "utilities.h" // get_checksum

/*
#include "dirfile_writer.h"
#include "data_output.h"
#include "file_helper.h"

//#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
*/

// Todo - this should be callback based for most flexibility and reuse among platforms

// Todo - check if function pointer type enforces safety - try registering an incompatible callback


// This should run forever
void packet_parser(uint8_t *buffer,
                   size_t buffer_size,
                   read_callback_t read_callback,
                   void *read_params,
                   packet_handler_callback_t packet_handler_callback,
                   void *packet_handler_params)
{
    size_t bytes_read;
    size_t rx_buf_bytes = 0;
    // Read() is blocking
    // Zero bytes read means eof / closed by writer
    // Appends to partially filled buffer to account for partial packet writes / reads
    while ((bytes_read = read_callback(buffer + rx_buf_bytes,      // where to read
                                       buffer_size - rx_buf_bytes, // maximum number of bytes to read
                                       read_params)))              // additional parameters necessary to read (file descriptor, etc.)
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
            if (magic_num != *(uint32_t*)(buffer + bytes_consumed))
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


            packet_data_t *packet = (packet_data_t*)(buffer + bytes_consumed);
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

            uint8_t embedded_checksum = *(uint8_t*)(buffer + bytes_consumed + required_packet_size - sizeof(checksum));
            if (checksum != embedded_checksum)
            {
                // out of sync
                // re-scan for magic num
                printf("lost sync (checksum mismatch %u != %u embedded)", checksum, embedded_checksum);
                out_of_sync = true;
                continue;
            }

            packet_handler_callback(packet, packet_handler_params);
            bytes_consumed += required_packet_size;
        }

        // Eliminate consumed bytes
        rx_buf_bytes -= bytes_consumed;

        // Shift remaining bytes to beginning of buffer if necessary
        if (rx_buf_bytes)
        {
            memmove(buffer, buffer + bytes_consumed, rx_buf_bytes);
        }
    }

    printf("read zero bytes (EOF) - packet_parser exiting\n");
}