#include "packet_data_type.h"

// extern (implicit with const)
const uint32_t magic_num = 0xFEEDCAFE;

const size_t packet_payload_size[] = {
    [UNINITIALISED] = 0,
    [COMM_HEARTBEAT] = 0,
    [MOTOR_VALUES] = sizeof(motor_values_t),
    [PID_PARAMETERS] = sizeof(pid_param_t),
    [PID_CONFIGUARTION] = sizeof(pid_config_t),
};
