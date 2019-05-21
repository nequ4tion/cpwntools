#include <cpwn.h>

int
main(int argc, char *const *argv)
{
    u8 x = 8;
    u16 x1 = 16;

    /* the printhex and printdec macros work for every uint* and int* type from stdint.h */
    printhex(x);
    printdec(x);
    printhex(x1);
    printdec(x1);
}