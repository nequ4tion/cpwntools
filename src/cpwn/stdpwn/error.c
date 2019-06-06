/* Copyright (c) 2018 Nequ4tion */
/* See LICENSE for licensing information */

/* ***********************************************************
 * NAME: error.c
 * PURPOSE: Implement error handling
 *************************************************************/

#include "error.h"
#include "colors.h"
#include <stdio.h>
#include <stdlib.h>

#ifndef _WIN32
__attribute__((__noreturn__))
#endif /* _WIN32 */
void
PANIC(const char *functionname,
      const char *filename,
      const unsigned long long linenum)
{
  do
  {
    fprintf(stderr,
            "%s%s%s%s in %s%s%s at line %s%s%lld %s%sfailed.%s\n",
            colors.styles.bold,
            colors.strong_fg.magenta,
            functionname,
            colors.strong_fg.red,
            colors.strong_fg.yellow,
            filename,
            colors.strong_fg.red,
            colors.styles.reset,
            colors.normal_fg.blue,
            linenum,
            colors.strong_fg.red,
            colors.styles.bold,
            colors.styles.reset);
    exit(-1);
  } while (1); /* Keep the compiler happy. */
}
