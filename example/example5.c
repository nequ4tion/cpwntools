#include <cpwn.h>

int
main(int argc, char *const *argv)
{
    char *cstr1 = cstr.from_nstr("Hello");
    char *cstr2 = cstr.from_nstr(" World!");

    cstr.append(&cstr1, cstr2);

    cstr.print(cstr1);

    cstr.dealloc(&cstr1);
    cstr.dealloc(&cstr2);
}