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

#include "data_gen.h"
#include "bbb_packet_handling.h"

int main(int argc, char **argv)
{
    printf("starting\n");

    char* input_path = "combo_structs";
    char* output_dirfile_path = "demo_dir";

    // Override input path
    if (argc > 1)
    {
        input_path = argv[1];
        printf("input path changed to %s\n", input_path);
    }

    int retval; // pthread_create return value

#define GEN_ENABLE 0
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
    pthread_t bbb_packet_parsing_thread;

    bbb_packet_parsing_task_params_t packet_parsing_task_params;
    packet_parsing_task_params.input_path = input_path;
    packet_parsing_task_params.output_dirfile_path = output_dirfile_path;

    if ((retval = pthread_create(&bbb_packet_parsing_thread, NULL, bbb_packet_parsing_task, (void *)&packet_parsing_task_params)))
    {
        printf("pthread create issue %s\n", strerror(retval));
        abort();
    }
    printf("past pthread create\n");

    // Todo - task to setup ncurses UI in this or separate task, and write commands back to source of structs stream
    // Printf logs need to be replaced with log to file. Will need to pass this path into tasks.

#if GEN_ENABLE
    pthread_join(datagen_thread, NULL);
#endif

    pthread_join(bbb_packet_parsing_thread, NULL);
    printf("exiting\n");

    return 0;
}
