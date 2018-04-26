#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#include "getdata.h"

/*
Documentation:
http://getdata.sourceforge.net/dirfile.html
https://manpages.debian.org/testing/libgetdata-doc/index.html
http://getdata.sourceforge.net/getdata.html#data
https://kst-plot.kde.org/docbook/html/workingwithdata.html

apt install libgetdata-dev

gcc -g getdata_dirfile.c -lgetdata; and ./a.out

*/

int main()
{
    printf("hello\n");
    DIRFILE *df;

    // GD_TRUNC to clear out directory
    df = gd_open("dir", GD_RDWR | GD_CREAT | GD_TRUNC | GD_VERBOSE | GD_PRETTY_PRINT);
    //df = gd_open("dir", GD_RDWR | GD_VERBOSE | GD_PRETTY_PRINT);

    if (gd_error(df))
    {
        char *er = gd_error_string(df, NULL, 0);
        printf("gd_open error: %s\n", er);
        free(er);
    }

    gd_add_raw(df, "data", GD_FLOAT32, 1, 0);

    assert(sizeof(float) == 4);

    float fval;

    for (int i = 0; i < 1000; i++)
    {
        fval = (float)i;

        // Could really simplify this whole thing by writing directly to data files
        int n = gd_putdata(df,         // dirfile
                           "data",     // field name
                           i,          // start frame offset, this needs to be incremented
                           0,          // start sample offset
                           1,          // num frames to write
                           0,          // num samples to write added to frames
                           GD_FLOAT32, // type
                           &fval);     // value

        assert(n == 1);

        gd_sync(df, NULL); // Force write to file

        printf("%d\n", i);

        usleep(1E5); // sleep 100ms
    }

    printf("closing file\n");

    gd_close(df);
}