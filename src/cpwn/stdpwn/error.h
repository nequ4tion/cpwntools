/* Copyright (c) 2018 Nequ4tion */
/* See LICENSE for licensing information */

/* ***********************************************************
 * NAME: error.h
 * PURPOSE: Implements error handling
 *************************************************************/

#ifndef ERROR_H
#define ERROR_H
#include "colors.h"
#include <stdio.h>
#include <stdlib.h>

__attribute__ ((__noreturn__)) void
PANIC (const char *functionname, const char *filename,
       const unsigned long long linenum);

#endif /* ERROR_H */
