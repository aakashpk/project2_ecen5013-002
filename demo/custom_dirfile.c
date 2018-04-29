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

#include "data_gen.h"
#include "packet_parser.h"




int main(void)
{
    printf("starting\n");



    char input_path[] = "combo_structs";
    char dirfile_output_path[] = "demo_dir";

#define GEN_ENABLE 1
#define GEN_BLOCK 0
#if GEN_ENABLE
    // datagen for testing
    pthread_t datagen_thread;

    int retval;
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

    // Todo - task to setup ncurses UI in this or separate task, and write commands back to source of structs stream
    // Printf logs need to be replaced with log to file. Will need to pass this path into tasks.




#if GEN_ENABLE
    pthread_join(datagen_thread, NULL);
#endif

    return 0;
}
