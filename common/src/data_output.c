#include "data_output.h"
#include "file_helper.h"

// data output write functions
void data_output_write_file(data_output_t *output, char* data, size_t len)
{
    fwrite_flush(output->output_fp, data, len, 0); // no flush here
}

void data_output_write_serial(data_output_t *output, char* data, size_t len)
{
    uart_send_n((void *)data,len);
}

void data_output_write_socket(data_output_t *output, char* data, size_t len)
{
    // todo socket write
}

// data output flush functions
void data_output_flush_file(data_output_t *output)
{
    assert(0 == fflush(output->output_fp));
}

void data_output_flush_serial(data_output_t *output)
{
    // todo serial flush
}

void data_output_flush_socket(data_output_t *output)
{
    // todo socket flush
}

// data output open functions
void data_output_open_file(data_output_t *output, char* name)
{
    #ifdef BBB
    output->output_fp = fopen_check(name, "w");
    #endif
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

void data_output_write(data_output_t *output, char* data, size_t len)
{
    // write function pointers
    static void (*const data_output_write_func[])(data_output_t * output, char *data, size_t len) = {
        [OUTPUT_TO_FILE] = data_output_write_file,
        [OUTPUT_TO_SERIAL] = data_output_write_serial,
        [OUTPUT_TO_SOCKET] = data_output_write_socket,
    };

    // Question: Not sure why static must be next to void, rather than next to const above.
    // Seems like that would result in an array of "static void functions",
    // rather than a "static array" of "void functions".

    data_output_write_func[output->current_output_mode](output, data, len);
}

void data_output_open(data_output_t *output, char* name)
{
    // open function pointers
    static void (*const data_output_open_func[])(data_output_t * output, char *name) = {
        [OUTPUT_TO_FILE] = data_output_open_file,
        [OUTPUT_TO_SERIAL] = data_output_open_serial,
        [OUTPUT_TO_SOCKET] = data_output_open_socket,
    };

    data_output_open_func[output->current_output_mode](output, name);
}

void data_output_close(data_output_t *output)
{
    // close function pointers
    static void (*const data_output_close_func[])(data_output_t * output) = {
        [OUTPUT_TO_FILE] = data_output_close_file,
        [OUTPUT_TO_SERIAL] = data_output_close_serial,
        [OUTPUT_TO_SOCKET] = data_output_close_socket,
    };

    data_output_close_func[output->current_output_mode](output);
}

void data_output_flush(data_output_t *output)
{
    // flush function pointers
    static void (*const data_output_flush_func[])(data_output_t * output) = {
        [OUTPUT_TO_FILE] = data_output_flush_file,
        [OUTPUT_TO_SERIAL] = data_output_flush_serial,
        [OUTPUT_TO_SOCKET] = data_output_flush_socket,
    };

    data_output_flush_func[output->current_output_mode](output);
}
