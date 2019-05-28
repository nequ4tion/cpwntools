/* Copyright (c) 2018 Nequ4tion */
/* See LICENSE for licensing information */

/* ***********************************************************
 * NAME: cstring.c
 * PURPOSE: Implement non-nul-terminated strings in C
 *************************************************************/

#include "cstring.h"
#include "alloc.h"
#include "stdtypes.h"
#include <stdio.h>
#include <string.h>

struct STRING_HEADER
{
  size_t strlen;
  size_t capacity;
};

#define GET_HEADER(str) &(((struct STRING_HEADER*)str)[-1])
#define GET_STR(header) (char*)(&header[1])


static char*
cstr_new_str(size_t capacity)
{
  /* enough memory for the header and the string */
  const size_t full_size = (sizeof(struct STRING_HEADER) + capacity);
  /* a pointer to the header that holds information about the string */
  struct STRING_HEADER* header;
  /* the actual user data the string holds */
  char* string_data;

  /* initialize the memory with for the cstr with zero */
  header = (struct STRING_HEADER*)alloc.zero_alloc(full_size);

  /* check for integer overflow, just in case */
  if (full_size < capacity)
    return NULL;

  /* the initial string is 0 bytes long */
  header->strlen = 0;
  /* the initial capacity is set by the input parameter */
  header->capacity = capacity;

  /* first byte after the header */
  string_data = GET_STR(header);

  return string_data;
}

static void
cstr_resize(char** str, size_t capacity)
{
  /* the string header that holds information about the string */
  struct STRING_HEADER* header =  GET_HEADER(*str);

  /* do not reallocate if the capacity is already as big as the input value */
  if (header->capacity >= capacity)
    return;
  else {
    /* reallocate the string; this copies the data to the newly allocated memory */
    header = (struct STRING_HEADER*)alloc.realloc(
      (void*)header, /* a pointer to the header and start of the memory */
      capacity + sizeof(struct STRING_HEADER) /* enough memory for the new capacity and the header */
      );
    /* set the new capacity for the header */
    header->capacity = capacity;
    /* dereference the string pointer and set it to the string data from the header */
    *str = GET_STR(header);
  }
}

/**
 * @brief Grows the cstring by doubling its capacity.
 * 
 * @param str A pointer to the cstring.
 */
static void
cstr_grow(char **str)
{
  /* the string header that holds all the information about the string */
  const struct STRING_HEADER* header = GET_HEADER(*str);
  /* the new capacity; the current capacity multiplied by 2 */
  const size_t new_capacity = header->capacity * 2;
  
  /* resize with the new capacity */
  cstr_resize(str, new_capacity);
}

/**
 * @brief Copies the buffer to a cstring; allocates memory.
 * 
 * @param str A pointer to an initialized cstring.
 * @param buf The buffer to be copied.
 * @param buflen The size of the buffer in bytes.
 */
static void
copy_to_str(char** str, const char* buf, size_t buflen)
{
  /* the string header that holds all the information about the string */
  struct STRING_HEADER* header;

  /* resize the string to the size of the buffer */
  cstr_resize(str, buflen);
  /* the memory address might change, so this will need to be set to the changed address */
  header = GET_HEADER(*str);

  /* copy the memory to the string data, the address of the newly allocated memory in *str has been changed by cstr_resize */
  memcpy(*str, buf, buflen);
  /* set the strlen to the length of the buffer */
  header->strlen = buflen;
}

static char*
cstr_from_nstr(const char* str)
{
  /* find the length of the NUL-terminated string with string.h's strlen */
  const size_t len = strlen(str);
  /* allocate a string of the determined size */
  char* constructed_str = cstr_new_str(len);

  /* copy the NUL-terminated string to the allocated cstring, excluding the NUL-terminator */
  copy_to_str(&constructed_str, str, len);
  return constructed_str;
}

static char*
cstr_from_buf(const char* buf, size_t buflen)
{
  /* allocate a new cstring with the capacity of buflen */
  char* str = cstr_new_str(buflen);

  /* copy the buffer to the cstring */
  copy_to_str(&str, buf, buflen);
  return str;
}

static size_t
cstr_strlen(const char* str)
{
  return (GET_HEADER(str))->strlen;
}

static void
cstr_dealloc(char** str)
{
  /* dereference the string pointer */
  char* real_ptr = *str;
  void *allocated_memory_block;

  /* if the pointer is already freed it will be equal to NULL */
  if (real_ptr == NULL)
    return;

  /* the allocated_memory_block pointer points to the beginning of the header */
  allocated_memory_block = (void*)GET_HEADER(real_ptr);

  /* free the memory */
  alloc.free(allocated_memory_block);
  /* set the str pointer to NULL so double frees happen less often */
  *str = NULL;
}

static void
cstr_append_data(char** str, const char* buf, size_t buflen)
{
  /* the string header that holds all the information about the string */
  struct STRING_HEADER* header = GET_HEADER(*str);
  const size_t new_capacity = header->strlen + buflen;
  char* strptr;

  if (header->capacity < 20) {
    /* if the capacity is smaller than 20 bytes there is no point in growing the string */
    cstr_resize(str, new_capacity);
  } else {
    /* grow the string by a factor of 2 on every iteration, this will allocated more memory
     * than needed; this speculates on the possibility, that there may be future allocations
     * thus lowering the amount of future context switches during execution
     */
    while (header->capacity < new_capacity) {
      cstr_grow(str);
      header = GET_HEADER(*str);
    }
  }

  /* after the reallocation of the memory pointers might have changed */
  strptr = *str;

  /* here as well: pointers might have changed */
  header = GET_HEADER(strptr);

  /* offset after the last element of the string, since we start counting from 0 */
  memcpy(&(strptr[header->strlen]), &buf[0], buflen);

  /* set the new strlen to new_capacity since that is what has been copied */
  header->strlen = new_capacity;
}

static void
cstr_append(char** str, const char* strbuf)
{
  /* same as cstr_append_data but using the data stored in the cstring header of strbuf */
  cstr_append_data(str, strbuf, (GET_HEADER(strbuf))->strlen);
}

static char*
cstr_strdup(const char* str)
{
  /* allocates a cstring of the same size */
  char* ret = cstr_new_str((GET_HEADER(str))->capacity);
  /* copy the contents of the original string */
  copy_to_str(&ret, str, (GET_HEADER(str))->strlen);
  return ret;
}

static void
cstr_fprint(FILE* stream, const char* str)
{
  fwrite(str, sizeof(char), (GET_HEADER(str))->strlen, stream);
}

static void
cstr_print(const char* str)
{
  char newline = '\n';
  fwrite(str, sizeof(char), (GET_HEADER(str))->strlen, stdout);
  fwrite(&newline, sizeof(newline), 1, stdout);
}

static void
cstr_fill_bytes(char** cstring, char byte, size_t len)
{
  struct STRING_HEADER* header;

  /* resize the string with the given length */
  cstr_resize(cstring, len);
  /* initialize the memory of the cstring with byte */
  memset((void*)*cstring, (int)byte, len);
  /* dereference here since the memory address might have changed in cstr_resize */
  header = GET_HEADER(*cstring);
  /* set the length of the string to the newly allocated length */
  header->strlen = len;
}

static size_t
cstr_capacity(char* str)
{
  struct STRING_HEADER* header;

  header = GET_HEADER(str);

  return header->capacity;
}

const struct cstring_h cstr = { .from_nstr = cstr_from_nstr,
                                .new_str = cstr_new_str,
                                .from_buf = cstr_from_buf,
                                .strlen = cstr_strlen,
                                .buftostr = copy_to_str,
                                .dealloc = cstr_dealloc,
                                .resize = cstr_resize,
                                .append_data = cstr_append_data,
                                .append = cstr_append,
                                .strdup = cstr_strdup,
                                .fprint = cstr_fprint,
                                .print = cstr_print,
                                .fill_bytes = cstr_fill_bytes,
                                .capacity = cstr_capacity };
