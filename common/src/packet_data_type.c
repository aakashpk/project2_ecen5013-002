#include <stddef.h> // size_t
#include "packet_data_type.h"

// may be able to just put const in header

// extern (implicit with const)
//const uint32_t magic_num = 0xFEEDCAFE;
// const having conflicts with non const write wrappers. non-const is faster fix
uint32_t magic_num = 0xFEEDCAFE;

const size_t packet_payload_size[] = {
    [UNINITIALISED] = 0,
    [COMM_HEARTBEAT] = 0,
    [MOTOR_VALUES] = sizeof(motor_values_t),
    [PID_PARAMETERS] = sizeof(pid_param_t),
    [PID_CONFIGUARTION] = sizeof(pid_config_t),
};

const char *packet_type_strings[] = {
    "UNINITIALISED",
    "HEARTBEAT",
    "MOTOR_VALUES",
    "PID_PARAMETERS",
    "PID_CONFIGUARTION",
};
