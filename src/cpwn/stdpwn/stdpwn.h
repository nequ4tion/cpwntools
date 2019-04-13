/* Copyright (c) 2018 Nequ4tion */
/* See LICENSE for licensing information */

/* ***********************************************************
 * NAME: stdpwn.h
 * PURPOSE: Include the libraries
 *************************************************************/

/*! \file stdpwn.h */ 

#ifndef STDPWN_H
#define STDPWN_H

#include "stdtypes.h"

#include "archdefs.h"

#include "alloc.h"
#include "bignum.h"
#include "boolean.h"
#include "colors.h"
#include "cstring.h"
#include "error.h"

void
stdpwn_printhex_u8(uint8_t x);
void
stdpwn_printhex_u16(uint16_t x);
void
stdpwn_printhex_u32(uint32_t x);
void
stdpwn_printhex_u64(uint64_t x);

void
stdpwn_printhex_i8(int8_t x);
void
stdpwn_printhex_i16(int16_t x);
void
stdpwn_printhex_i32(int32_t x);
void
stdpwn_printhex_i64(int64_t x);

void
stdpwn_print_u8(uint8_t x);
void
stdpwn_print_u16(uint16_t x);
void
stdpwn_print_u32(uint32_t x);
void
stdpwn_print_u64(uint64_t x);

void
stdpwn_print_i8(int8_t x);
void
stdpwn_print_i16(int16_t x);
void
stdpwn_print_i32(int32_t x);
void
stdpwn_print_i64(int64_t x);

/*! \def printhex(integer)
    \brief Prints an integer in its hexadecimal representation.
*/
#define printhex(integer)                                                      \
  ((_Generic((integer), uint8_t                                                \
             : stdpwn_printhex_u8, uint16_t                                    \
             : stdpwn_printhex_u16, uint32_t                                   \
             : stdpwn_printhex_u32, uint64_t                                   \
             : stdpwn_printhex_u64, int8_t                                     \
             : stdpwn_printhex_i8, int16_t                                     \
             : stdpwn_printhex_i16, int32_t                                    \
             : stdpwn_printhex_i32, int64_t                                    \
             : stdpwn_printhex_i64))(integer))


/*! \def printdec(integer)
    \brief Prints an integer in its decimal representation.
*/
#define printdec(integer)                                                      \
  ((_Generic((integer), uint8_t                                                \
             : stdpwn_print_u8, uint16_t                                       \
             : stdpwn_print_u16, uint32_t                                      \
             : stdpwn_print_u32, uint64_t                                      \
             : stdpwn_print_u64, int8_t                                        \
             : stdpwn_print_i8, int16_t                                        \
             : stdpwn_print_i16, int32_t                                       \
             : stdpwn_print_i32, int64_t                                       \
             : stdpwn_print_i64))(integer))

#endif /* STDPWN_H */
