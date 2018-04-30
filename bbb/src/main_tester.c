/**
 * @brief 
 * 
 * @file main_tester.c
 * @author Aakash
 * @author Miles
 * @date 2018-04-29
 */
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
/*
#include <stdint.h>
#include <assert.h>
#include <limits.h>
#include <fcntl.h>
*/

//#include "packet_data_type.h"

#include "data_gen.h"
#include "packet_parser.h"




int main(void)
{
    printf("starting\n");

    char* input_path = "combo_structs";
    char* dirfile_output_path = "demo_dir";

    int retval; // pthread_create return value

#define GEN_ENABLE 1
#define GEN_BLOCK 0
#if GEN_ENABLE
    // datagen for testing
    pthread_t datagen_thread;

    if ((retval = pthread_create(&datagen_thread, NULL, datagen_task, (void *)input_path)))
    {
        printf("pthread create issue %s\n", strerror(retval));
        abort();
    }

    // File read will return 0 bytes at EOF and trigger loop exit
    // This is fine for testing. Other communication schemes will support better blocking.

    sleep(1); // allow file to be created while debugging

#if GEN_BLOCK
    // Force continuous or time-limited data generation
    pthread_join(datagen_thread, NULL);
#endif

#endif // GEN_ENABLE

    // Todo - call packet parser function/task
    pthread_t packet_parser_thread;

    packet_parser_params_t packet_parser_param;
    packet_parser_param.input_path = input_path;
    packet_parser_param.output_dir = dirfile_output_path;

    if ((retval = pthread_create(&packet_parser_thread, NULL, packet_parser_task, (void *)&packet_parser_param)))
    {
        printf("pthread create issue %s\n", strerror(retval));
        abort();
    }

    // Todo - task to setup ncurses UI in this or separate task, and write commands back to source of structs stream
    // Printf logs need to be replaced with log to file. Will need to pass this path into tasks.

#if GEN_ENABLE
    pthread_join(datagen_thread, NULL);
#endif

    return 0;
}
