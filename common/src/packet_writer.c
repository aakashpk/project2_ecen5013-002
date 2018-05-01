#include "packet_writer.h"
#include "utilities.h"

void write_packet(packet_data_t *data,
                  write_callback_t write_callback,
                  flush_callback_t flush_callback,
                  void* additional_write_flush_params)
{
    size_t payload_len = packet_payload_size[data->header.packet_type];

    write_callback(&magic_num, sizeof(magic_num), additional_write_flush_params);
    write_callback(&data->header, sizeof(data->header), additional_write_flush_params);
    write_callback(&data->payload, payload_len, additional_write_flush_params);

    uint8_t checksum = 0;
    checksum = get_checksum((uint8_t*)&data->header, sizeof(data->header), checksum);
    checksum = get_checksum((uint8_t*)&data->motor_values, payload_len, checksum);
    write_callback(&checksum, sizeof(checksum), additional_write_flush_params);

    // Todo - may want to let flush be optional here,
    // or with reduced flush rate
    if (flush_callback)
    {
        flush_callback(additional_write_flush_params);
    }
}
