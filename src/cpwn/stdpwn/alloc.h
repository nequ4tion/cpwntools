/* Copyright (c) 2018 Nequ4tion */
/* See LICENSE for licensing information */

/* ***********************************************************
 * NAME: alloc.h
 * PURPOSE: Implement allocations
 *************************************************************/

#ifndef ALLOC_H
#define ALLOC_H

#include "error.h"
#include <stddef.h>
#include <stdlib.h>

/**
 * @brief The `alloc` module consists 
 * of a set of functions that allocate 
 * or deallocate dynamic memory.
 * Functions in this module can be called by
 * invoking alloc.`functionname`.
 */
struct alloc_h
{
  /**
   * @brief The `alloc.malloc` function allocates 
   * a chunk of memory that is equal or 
   * larger than `size` bytes. Works similar to 
   * `stdlib.h`'s `malloc` function.
   * 
   * @param size Number of bytes to allocate.
   * 
   * @return void* A pointer to the allocated memory.
   * 
   * @remark This function panics whenever the 
   * underlying malloc returns NULL.
   */
  void* (*malloc)(size_t);
  /**
   * @brief The `alloc.zero_alloc` function 
   * allocates `size` bytes of memory. 
   * It then initializes the allocated memory with zero.
   * 
   * @param size Number of bytes to allocate.
   * 
   * @return void* A pointer to the allocated memory.
   * 
   * @remark This function panics whenever the 
   * underlying calloc returns NULL.
   */
  void* (*zero_alloc)(size_t);
  /**
   * @brief The `alloc.zero_allocarray` function
   * allocates `nmemb` elements of `size` bytes each. 
   * It initializes the allocated memory with zero.
   * 
   * @param nmemb Number of elements.
   * @param size The size of each element.
   * 
   * @return void* A pointer to the allocated memory.
   * 
   * @remark This function panics whenever the 
   * underlying calloc returns NULL.
   */
  void* (*zero_allocarray)(size_t, size_t);
  /**
   * @brief The `alloc.realloc` function reallocates 
   * the memory that `ptr` points to and returns 
   * the newly allocated chunk. In practice this means, 
   * that it, when needed, frees the memory, allocates 
   * memory larger than or equal to `size` bytes and 
   * returns the result of that allocation.
   * 
   * @param ptr A pointer to the previously allocated memory.
   * @param size Number of bytes to allocate.
   * 
   * @return void* A pointer to the allocated memory.
   * 
   * @remark This function panics whenever the 
   * underlying calloc returns NULL.
   */
  void* (*realloc)(void*, size_t);
  /**
   * @brief The `alloc.free` function 
   * deallocates the allocated memory that 
   * `ptr` points to.
   * 
   * 
   * @param ptr A pointer to the memory to be free'd
   */
  void (*free)(void*);
  /**
   * @brief The `alloc.sfree` function takes an 
   * argument which is a pointer to a pointer 
   * that points to dynamically allocated memory, 
   * and similarly to \link alloc_h.free alloc.free\endlink, 
   * frees the memory. After freeing the memory 
   * the original pointer is modified to be `NULL`.
   * 
   * @param ptr A pointer to a pointer to the memory 
   * to be free'd.
   */
  void (*sfree)(void**);
};

extern const struct alloc_h alloc;

#endif /* ALLOC_H */
