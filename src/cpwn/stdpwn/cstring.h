/* Copyright (c) 2018 Nequ4tion */
/* See LICENSE for licensing information */

/* ***********************************************************
 * NAME: cstring.h
 * PURPOSE: Define non-nul-terminated strings in C
 *************************************************************/

#ifndef CSTRING_H
#define CSTRING_H

#include "alloc.h"
#include "stdtypes.h"
#include <stdio.h>
#include <string.h>

struct cstring_h
{
  /**
   * @brief Creates a string from a NUL-terminated C-style string.
   *
   * @param str The string
   * @return char * A pointer to an usable string. Do not use free()!
   * Use dealloc() to deallocate it!
   */
  char* (*from_nstr)(const char*);
  /**
   * @brief Creates a new string with the specified capacity.
   *
   * @param capacity The size that will be allocated in bytes.
   * @return char * Same as from_cstr
   */
  char* (*new_str)(size_t);
  /**
   * @brief Creates a string from a specified block of memory.
   *
   * @param buf Pointer to a block of memory.
   * @param buflen The size of the block of memory in bytes.
   * @return char * Same as from_cstr
   */
  char* (*from_buf)(const char*, size_t);
  /**
   * @brief Returns the length of a string. NOT ITS CAPACITY!!!
   *
   * @param str The string which length should be returned.
   * @return size_t The length of the string.
   */
  size_t (*strlen)(const char*);
  /**
   * @brief Copies a buf to an already allocated string. This lets your reuse
   * strings.
   *
   * @param str The string to copy into.
   * @param buf The buffer that will be copied from.
   */
  void (*buftostr)(char**, const char*, size_t);
  /**
   * @brief Deallocates a string. No other function should be used to do
   * that.
   *
   * @param str A pointer to a string pointer. This is done to avoid double
   * frees by setting *str to NULL.
   */
  void (*dealloc)(char**);
  /**
   * @brief Adjusts the capacity.
   *
   * @param str The string to resize.
   * @param capacity The new capacity.
   * @return char * Returns the newly allocated string. (Dismiss the old str
   * pointer, DO NOT USE IT!!!)
   */
  void (*resize)(char**, size_t);
  /**
   * @brief Appends a buffer of specified length to the string.
   *
   * @param str A pointer to a string. (reference it first with &str)
   * @param buf The buffer to append from.
   * @param buflen The size of the buffer.
   */
  void (*append_data)(char**, const char*, size_t);
  /**
   * @brief Appends a string to another.
   *
   * @param str A pointer to a str. (reference it first with &)
   * @param strbuf The other string to copy from.
   */
  void (*append)(char**, const char*);
  /**
   * @brief Duplicates a string.
   *
   * @param str The string to duplicate.
   * @return char * Returns the duplicated string.
   */
  char* (*strdup)(const char*);
  /**
   * @brief Writes the string to the specified FILE stream. NULL bytes will
   * not be ignored.
   *
   * @param stream The FILE stream to write to
   * @param str The string to write to the FILE
   */
  void (*fprint)(FILE*, const char*);
  /**
   * @brief Writes the string to stdout. Appends a newline.
   *
   * @param str The string to print.
   */
  void (*print)(const char*);
  /**
   * @brief Fills the string with `len` bytes of the kind `byte` and allocates
   * new space, if necessary.
   */
  void (*fill_bytes)(char**, char, size_t);
  /**
   * @brief Returns the capacity of the given string.
   * @param The string of which the capacity will be returned.
   */
  size_t (*capacity)(char *);
};

extern const struct cstring_h cstr;

#endif /* CSTRING_H */
