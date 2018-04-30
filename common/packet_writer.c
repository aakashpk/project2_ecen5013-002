#include "packet_writer.h"
#include "utilities.h"

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

    // Todo - may want to let flush be optional here,
    // or with reduced flush rate
    data_output_flush(output);
}