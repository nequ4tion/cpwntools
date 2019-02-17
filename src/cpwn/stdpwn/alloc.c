/* Copyright (c) 2018 Nequ4tion */
/* See LICENSE for licensing information */

/* ***********************************************************
 * NAME: alloc.c
 * PURPOSE: Implement allocations
 * NOTE: This library panics if *alloc returns NULL.
 *************************************************************/

#include "alloc.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>

static void
alloc_out_of_memory(const char* funcname, int line)
{
  PANIC(funcname, __FILE__, line);
}

#define CHECK_NULL_AND_PANIC(x, fname)                                         \
  if (x == NULL)                                                               \
    alloc_out_of_memory(fname, __LINE__);                                      \
  return x

static void*
alloc_malloc(size_t size)
{
  void* ret = malloc(size);
  CHECK_NULL_AND_PANIC(ret, "alloc_malloc");
}

static void*
alloc_zero_alloc_array(size_t nmemb, size_t size)
{
  void* ret = calloc(nmemb, size);
  CHECK_NULL_AND_PANIC(ret, "alloc_zero_alloc_array");
}

static void*
alloc_zero_alloc(size_t size)
{
  void* ret = alloc_zero_alloc_array(size, sizeof(unsigned char));
  CHECK_NULL_AND_PANIC(ret, "alloc_zero_alloc");
}

static void*
alloc_realloc(void* ptr, size_t size)
{
  printf("ptr: %p\n", ptr);
  void* ret = realloc(ptr, size);
  printf("ret: %p\n", ret);
  CHECK_NULL_AND_PANIC(ret, "alloc_realloc");
}

static void
alloc_free(void* ptr)
{
  if (ptr == NULL) {
#ifdef DEBUG
    /* catch NULL pointers passed to frees in debug mode */
    PANIC(__FUNCTION__, __FILE__, __LINE__);
#else
    /* if we are not in debug mode this case will be ignored for stability */
    return;
#endif /* DEBUG */
  }
  free(ptr);
}

static void
alloc_sfree(void** ptr)
{
  alloc_free(*ptr);
  *ptr = NULL;
}

const struct alloc_h alloc = { .malloc = alloc_malloc,
                               .zero_alloc = alloc_zero_alloc,
                               .zero_allocarray = alloc_zero_alloc_array,
                               .realloc = alloc_realloc,
                               .free = alloc_free,
                               .sfree = alloc_sfree };
