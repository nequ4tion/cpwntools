/* Copyright (c) 2018 Nequ4tion */
/* See LICENSE for licensing information */

/* ***********************************************************
 * NAME: cstring.h
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
alloc_str(size_t capacity)
{
  size_t full_size = (sizeof(struct STRING_HEADER) + capacity);
  struct STRING_HEADER* header;
  char* string_data;

  /* check for integer overflow */
  if (full_size < capacity)
    return NULL;

  header = (struct STRING_HEADER*)alloc.zero_alloc(full_size);

  header->strlen = 0;
  header->capacity = capacity;

  /* first byte after the header */
  string_data = GET_STR(header);

  return string_data;
}

static char*
cstr_resize(char* str, size_t capacity)
{
  struct STRING_HEADER* header;

  header = GET_HEADER(str);
  if (header->capacity > capacity)
    return str;

  header = (struct STRING_HEADER*)alloc.realloc(
    (void*)header, capacity + sizeof(struct STRING_HEADER));
  header->capacity = capacity;

  return GET_STR(header);
}

static void
copy_to_str(char* str, const char* buf, size_t buflen)
{
  struct STRING_HEADER* header;
  header = GET_HEADER(str);

  if (header->capacity < buflen) {
    cstr_resize(str, buflen);
    header = GET_HEADER(str);
  }

  memcpy(str, buf, buflen);
  header->strlen = buflen;
}

static char*
cstr_from_nstr(const char* str)
{
  size_t len;
  char* constructed_str;

  len = strlen(str);
  constructed_str = alloc_str(len);
  copy_to_str(constructed_str, str, len);
  return constructed_str;
}

static char*
cstr_new_str(size_t capacity)
{
  return alloc_str(capacity);
}

static char*
cstr_from_buf(const char* buf, size_t buflen)
{
  char* str;

  str = alloc_str(buflen);
  copy_to_str(str, buf, buflen);
  return str;
}

static size_t
cstr_strlen(const char* str)
{
  return (GET_HEADER(str))->strlen;
}

static void
cstr_buftostr(char* str, const char* buf, size_t buflen)
{
  copy_to_str(str, buf, buflen);
}

static void
cstr_dealloc(char** str)
{
  char* real_ptr = *str;
  void* chunk_ptr = (void*)GET_HEADER(real_ptr);

  if (real_ptr == NULL)
    return;

  alloc.free(chunk_ptr);
  *str = NULL;
}

static void
cstr_append_data(char** str, const char* buf, size_t buflen)
{
  struct STRING_HEADER* header = GET_HEADER(*str);
  size_t new_capacity = header->strlen + buflen;
  char* strptr = *str;

  if (header->capacity < new_capacity)
    strptr = cstr_resize(*str, new_capacity);

  header = GET_HEADER(strptr);
  memcpy(&(strptr[header->strlen]), &buf[0], buflen);
  header->strlen = new_capacity;
  *str = strptr;
}

static void
cstr_append(char** str, const char* strbuf)
{
  cstr_append_data(str, strbuf, (GET_HEADER(strbuf))->strlen);
}

static char*
cstr_strdup(const char* str)
{
  char* ret = alloc_str((GET_HEADER(str))->capacity);
  copy_to_str(ret, str, (GET_HEADER(str))->strlen);
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
  fwrite(str, sizeof(char), (GET_HEADER(str))->strlen, stdout);
  fwrite("\n", 1, 1, stdout);
}

static void
cstr_fill_bytes(char* cstring, char byte, size_t len)
{
  struct STRING_HEADER* header;

  cstr_resize(cstring, len);
  memset((void*)cstring, (int)byte, len);
  header = GET_HEADER(cstring);
  header->strlen = len;
}

const struct cstring_h cstr = { .from_nstr = cstr_from_nstr,
                                .new_str = cstr_new_str,
                                .from_buf = cstr_from_buf,
                                .strlen = cstr_strlen,
                                .buftostr = cstr_buftostr,
                                .dealloc = cstr_dealloc,
                                .resize = cstr_resize,
                                .append_data = cstr_append_data,
                                .append = cstr_append,
                                .strdup = cstr_strdup,
                                .fprint = cstr_fprint,
                                .print = cstr_print,
                                .fill_bytes = cstr_fill_bytes };
