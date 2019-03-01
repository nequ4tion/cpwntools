/* Copyright (c) 2018 Nequ4tion */
/* See LICENSE for licensing information */

/* ***********************************************************
 * NAME: alloc.h
 * PURPOSE: Implement allocations
 * NOTE: This library panics if *alloc returns NULL.
 *************************************************************/

#ifndef ALLOC_H
#define ALLOC_H

#include "error.h"
#include <stddef.h>
#include <stdlib.h>

struct alloc_h
{
  void* (*malloc)(size_t);
  void* (*zero_alloc)(size_t);
  void* (*zero_allocarray)(size_t, size_t);
  void* (*realloc)(void*, size_t);
  void (*free)(void*);
  void (*sfree)(void**);
};

extern const struct alloc_h alloc;

#endif /* ALLOC_H */
