#include <cpwn.h>

int
main(int argc, char *const *argv)
{
    /* allocate from NUL-terminated string (nstr) */
    char *cstr1 = cstr.from_nstr("Hello");
    char *cstr2 = cstr.from_nstr(" World!");

    /* appends cstr2 to cstr1, does automatic memory allocation */
    cstr.append(&cstr1, cstr2);

    /* prints with a newline */
    cstr.print(cstr1);

    /* deallocates the string, takes a reference since it sets the pointer to NULL to avoid use after frees */
    cstr.dealloc(&cstr1);
    cstr.dealloc(&cstr2);
}