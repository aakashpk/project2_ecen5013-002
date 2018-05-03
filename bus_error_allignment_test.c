#include <stdio.h>

typedef struct
{
    char c;
    float f;
} thing;

int main()
{
    printf("starting\n");

    char buf[20];

    for (int i = 0; i < sizeof(buf); i++)
    {
        buf[i] = i;
    }

    float f;
    for (int i = 0; i < sizeof(buf) - sizeof(f); i++)
    {
        f = *(float *)(buf + i);
        printf("f %f\n", f);
    }

    thing *t;
    for (int i = 0; i < sizeof(buf) - sizeof(thing); i++)
    {
        t = (thing *)(buf + i);
        printf("t->f %f\n", t->f);
    }

}

