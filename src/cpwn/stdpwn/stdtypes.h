/* Copyright (c) 2018 Nequ4tion */
/* See LICENSE for licensing information */

/* ***********************************************************
 * NAME: stdtypes.h
 * PURPOSE: Define new types
 *************************************************************/

/*! \file stdtypes.h */

#ifndef STDTYPES_H
#define STDTYPES_H

#include <stdint.h>
#include <stdlib.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

#if defined(__GNUC__) && defined(__x86_64__)
#ifndef INT_128_DEFINED
typedef signed int128_t __attribute__((mode(TI)));
typedef unsigned uint128_t __attribute__((mode(TI)));
#define INT_128_DEFINED
#endif /* INT_128_DEFINED */
#endif /* __GNUC__ */

#ifdef unix
typedef size_t sz_t;
typedef ssize_t ssz_t;
#elif defined(_WIN32)
typedef size_t sz_t;
typedef i64 ssz_t;
#endif /* unix */

#endif /* STDTYPES_H */
