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

/**
 * @brief Exits the program and prints an error message.
 * 
 * @param functionname The name of the current function.
 * @param filename The name of the current file.
 * @param linenum The current line in which this function gets called.
 */
#ifndef _WIN32
__attribute__((__noreturn__))
#endif /* _WIN32 */
void
PANIC(const char *functionname,
      const char *filename,
      const unsigned long long linenum);

#endif /* ERROR_H */
